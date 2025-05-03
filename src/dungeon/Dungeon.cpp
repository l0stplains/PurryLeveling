#include "dungeon/Dungeon.hpp"

#include "rng/rng.hpp"

const string Dungeon::RANK_S       = "S";
const string Dungeon::RANK_A       = "A";
const string Dungeon::RANK_B       = "B";
const string Dungeon::RANK_C       = "C";
const string Dungeon::RANK_D       = "D";
const string Dungeon::RANK_E       = "E";
const string Dungeon::RANK_SPECIAL = "SPECIAL";

Dungeon::Dungeon(const string&        rank,
                 int                  playerLevel,
                 ItemManager&         itemManager,
                 MobLootConfigParser& mobLootConfigParser,
                 int                  gold,
                 int                  exp)
    : rank(rank),
      entranceFee(0),
      minPlayerLevel(0),
      initGold(gold),
      initExp(exp),
      playerLevel(playerLevel),
      isCleared(false),
      expMultiplier(1.0),
      goldMultiplier(1.0),
      itemManager(itemManager),
      mobLootConfigParser(mobLootConfigParser)
{
    setupDungeonProperties();
}

Dungeon::~Dungeon() {}

void Dungeon::setupDungeonProperties()
{
    if (rank == RANK_S)
    {
        entranceFee    = 1000;
        minPlayerLevel = 50;
        expMultiplier  = 5.0;
        goldMultiplier = 5.0;
    }
    else if (rank == RANK_A)
    {
        entranceFee    = 500;
        minPlayerLevel = 35;
        expMultiplier  = 4.0;
        goldMultiplier = 4.0;
    }
    else if (rank == RANK_B)
    {
        entranceFee    = 300;
        minPlayerLevel = 25;
        expMultiplier  = 3.0;
        goldMultiplier = 3.0;
    }
    else if (rank == RANK_C)
    {
        entranceFee    = 150;
        minPlayerLevel = 20;
        expMultiplier  = 2.0;
        goldMultiplier = 2.0;
    }
    else if (rank == RANK_D)
    {
        entranceFee    = 0;
        minPlayerLevel = 10;
        expMultiplier  = 1.5;
        goldMultiplier = 1.5;
    }
    else if (rank == RANK_E)
    {
        entranceFee    = 0;
        minPlayerLevel = 0;
        expMultiplier  = 1.0;
        goldMultiplier = 1.0;
    }
    else if (rank == RANK_SPECIAL)
    {
        entranceFee    = 0;
        minPlayerLevel = 0;
        expMultiplier  = 2.0;
        goldMultiplier = 2.0;
    }
}

void Dungeon::addChamber(const Chamber& chamber)
{
    chambers.push_back(chamber);
}

void Dungeon::setPlayerLevel(int level)
{
    playerLevel = level;
}

int Dungeon::getPlayerLevel() const
{
    return playerLevel;
}

int Dungeon::getEntranceFee() const
{
    return entranceFee;
}

int Dungeon::getMinPlayerLevel() const
{
    return minPlayerLevel;
}

string Dungeon::getRank() const
{
    return rank;
}

vector<Chamber> Dungeon::getChambers() const
{
    return chambers;
}

Chamber& Dungeon::getChamber(int index)
{
    return chambers[index];
}

void Dungeon::clearChamber(int index)
{
    if (index < 0 || index >= chambers.size())
    {
        return;
    }

    Chamber& chamber = chambers[index];

    if (chamber.getIsCleared())
    {
        return;
    }

    vector<Item> chamberLoot = chamber.clearChamber();

    for (const auto& item : chamberLoot)
    {
        mobLoot.push_back(item);
    }

    if (areAllChambersCleared())
    {
        isCleared = true;
    }
}

void Dungeon::setCleared(bool cleared)
{
    isCleared = cleared;
}

bool Dungeon::getIsCleared() const
{
    return isCleared;
}

void Dungeon::addLoot(const Item& item)
{
    dungeonLoot.push_back(item);
}

void Dungeon::addMobLoot(const Item& item)
{
    mobLoot.push_back(item);
}

vector<Item> Dungeon::getLoot() const
{
    return dungeonLoot;
}

vector<Item> Dungeon::getMobLoot() const
{
    return mobLoot;
}

void Dungeon::generateLoot()
{
    RNG rng;
    int numItems = 1;

    if (rank == RANK_S)
    {
        numItems = rng.generateInRange(3, 5);
    }
    else if (rank == RANK_A)
    {
        numItems = rng.generateInRange(2, 4);
    }
    else if (rank == RANK_B)
    {
        numItems = rng.generateInRange(2, 3);
    }
    else if (rank == RANK_C)
    {
        numItems = rng.generateInRange(1, 3);
    }
    else if (rank == RANK_D || rank == RANK_E)
    {
        numItems = rng.generateInRange(1, 2);
    }
    else if (rank == RANK_SPECIAL)
    {
        numItems = rng.generateInRange(4, 8);
    }

    int doubleChamberCount = countDoubleChambers();
    if (doubleChamberCount > 0)
    {
        numItems += doubleChamberCount;

        if (numItems > 10)
        {
            numItems = 10;
        }
    }

    if (rank == RANK_SPECIAL)
    {
        vector<Item> allItems = itemManager.getAllItems();

        if (allItems.empty())
        {
            return;
        }

        for (int i = 0; i < numItems; i++)
        {
            double rarityRoll = rng.generateProbability();
            char   targetRarity;

            if (rarityRoll < 0.01)
            {
                targetRarity = 'S';
            }
            else if (rarityRoll < 0.05)
            {
                targetRarity = 'A';
            }
            else if (rarityRoll < 0.15)
            {
                targetRarity = 'B';
            }
            else if (rarityRoll < 0.35)
            {
                targetRarity = 'C';
            }
            else if (rarityRoll < 0.65)
            {
                targetRarity = 'D';
            }
            else
            {
                targetRarity = 'E';
            }

            vector<Item> itemsOfRarity = itemManager.getItemsByRarity(targetRarity);

            if (itemsOfRarity.empty())
            {
                itemsOfRarity = allItems;
            }

            int randomIndex = rng.generateInRange(itemsOfRarity.size());
            dungeonLoot.push_back(itemsOfRarity[randomIndex]);
        }
    }
    else
    {
        char targetRarity = 'E';

        if (rank == RANK_S)
            targetRarity = 'S';
        else if (rank == RANK_A)
            targetRarity = 'A';
        else if (rank == RANK_B)
            targetRarity = 'B';
        else if (rank == RANK_C)
            targetRarity = 'C';
        else if (rank == RANK_D)
            targetRarity = 'D';

        vector<Item> itemsOfRarity = itemManager.getItemsByRarity(targetRarity);

        if (itemsOfRarity.empty())
        {
            itemsOfRarity = itemManager.getAllItems();
        }

        if (itemsOfRarity.empty())
        {
            return;
        }

        for (int i = 0; i < numItems; i++)
        {
            int randomIndex = rng.generateInRange(itemsOfRarity.size());
            dungeonLoot.push_back(itemsOfRarity[randomIndex]);
        }
    }
}

void Dungeon::generateMobLoot()
{
    for (auto& chamber : chambers)
    {
        chamber.generateMobLoot(mobLootConfigParser, itemManager);
    }
}

void Dungeon::clearLoot()
{
    dungeonLoot.clear();
}

void Dungeon::clearMobLoot()
{
    mobLoot.clear();
}

int Dungeon::getNumChambers() const
{
    return chambers.size();
}

double Dungeon::getExpMultiplier() const
{
    return expMultiplier;
}

double Dungeon::getGoldMultiplier() const
{
    return goldMultiplier;
}

void Dungeon::setExpMultiplier(double multiplier)
{
    expMultiplier = multiplier;
}

void Dungeon::setGoldMultiplier(double multiplier)
{
    goldMultiplier = multiplier;
}

bool Dungeon::areAllChambersCleared() const
{
    for (const Chamber& chamber : chambers)
    {
        if (!chamber.getIsCleared())
        {
            return false;
        }
    }
    return true;
}

void Dungeon::getPenalty(int& expPenalty, int& goldPenalty) const
{
    if (rank == RANK_S)
    {
        expPenalty  = 2000;
        goldPenalty = 800;
    }
    else if (rank == RANK_A)
    {
        expPenalty  = 1500;
        goldPenalty = 600;
    }
    else if (rank == RANK_B)
    {
        expPenalty  = 1000;
        goldPenalty = 400;
    }
    else if (rank == RANK_C)
    {
        expPenalty  = 750;
        goldPenalty = 300;
    }
    else if (rank == RANK_D)
    {
        expPenalty  = 500;
        goldPenalty = 200;
    }
    else if (rank == RANK_E)
    {
        expPenalty  = 250;
        goldPenalty = 100;
    }
    else if (rank == RANK_SPECIAL)
    {
        int clearedChambers = 0;
        for (const Chamber& chamber : chambers)
        {
            if (chamber.getIsCleared())
            {
                clearedChambers++;
            }
        }

        expPenalty  = 100 + (clearedChambers * 50);
        goldPenalty = 50 + (clearedChambers * 20);

        if (expPenalty > 1500)
            expPenalty = 1500;
        if (goldPenalty > 600)
            goldPenalty = 600;
    }
    else
    {
        expPenalty  = 100;
        goldPenalty = 50;
    }

    int doubleChamberCount = countDoubleChambers();
    if (doubleChamberCount > 0)
    {
        double multiplier = 1.0 + (doubleChamberCount * 0.1);
        expPenalty        = static_cast<int>(expPenalty * multiplier);
        goldPenalty       = static_cast<int>(goldPenalty * multiplier);
    }

    double levelScalingFactor = 1.0 + (playerLevel * 0.01);
    expPenalty                = static_cast<int>(expPenalty * levelScalingFactor);
    goldPenalty               = static_cast<int>(goldPenalty * levelScalingFactor);
}

int Dungeon::countDoubleChambers() const
{
    int count = 0;
    for (const Chamber& chamber : chambers)
    {
        if (chamber.getIsDoubleChamber())
        {
            count++;
        }
    }
    return count;
}

void Dungeon::setQuest(const Quest& newQuest)
{
    quest = newQuest;
}

Quest& Dungeon::getQuest()
{
    return quest;
}

const Quest& Dungeon::getQuest() const
{
    return quest;
}

bool Dungeon::isQuestCompleted() const
{
    return quest.getIsCompleted() && areAllChambersCleared();
}

bool Dungeon::updateKillQuestProgress(int mobsKilled)
{
    return quest.updateKillProgress(mobsKilled);
}

bool Dungeon::updateDamageQuestProgress(int damageDealt)
{
    return quest.updateDamageProgress(damageDealt);
}

int Dungeon::getQuestGoldReward() const
{
    if (isQuestCompleted())
    {
        return quest.getGoldReward();
    }
    return 0;
}

int Dungeon::getQuestExpReward() const
{
    if (isQuestCompleted())
    {
        return quest.getExpReward();
    }
    return 0;
}

string Dungeon::getQuestItemRewardId() const
{
    if (isQuestCompleted())
    {
        return quest.getItemRewardId();
    }
    return "";
}

Item Dungeon::getQuestRewardItem() const
{
    if (isQuestCompleted())
    {
        try
        {
            return itemManager.getItem(getQuestItemRewardId());
        }
        catch (const char* error)
        {
            return Item();
        }
    }
    return Item();
}