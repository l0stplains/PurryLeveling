#include "dungeon/Chamber.hpp"

#include <iostream>
#include <map>

#include "units/mobs/basics/Goblin.hpp"
#include "units/mobs/basics/Orc.hpp"
#include "units/mobs/basics/Skeleton.hpp"
#include "units/mobs/basics/Slime.hpp"

using namespace std;

/*
class MobUnit : public Unit
{
public:
    MobUnit(const std::string& name) : Unit(name) {}

    // Implement the pure virtual function from Unit
    void UseSkill(int skillId, ActionCompletionCallback callback = nullptr) override
    {
        std::cout << GetName() << " uses skill ID: " << skillId << std::endl;

        // Default behavior for mob skill
        if (callback)
        {
            callback();
        }
    }
};
*/

Chamber::Chamber(int          chamberNumber,
                 bool         isBossRoom,
                 double       difficultyMultiplier,
                 int          mobLevelMin,
                 int          mobLevelMax,
                 UnitManager& unitManager,
                 GameContext& gameContext)
    : chamberNumber(chamberNumber),
      isBossRoom(isBossRoom),
      isDoubleChamber(false),
      difficultyMultiplier(difficultyMultiplier),
      mobLevelMin(mobLevelMin),
      mobLevelMax(mobLevelMax),
      isCleared(false),
      goldReward(0),
      expReward(0),
      rng(),
      unitManager(unitManager),
      gameContext(gameContext)
{}

Chamber::~Chamber() {}

void Chamber::generateMobs(bool isBossRoom)
{
    if (isBossRoom)
    {
        int bossType = rng.generateInRange(4);

        string bossName;

        switch (bossType)
        {
            case 0:
                bossName = "Ogre";
                break;
            case 1:
                bossName = "Dark Knight";
                break;
            case 2:
                bossName = "Demon Lord";
                break;
            case 3:
                bossName = "Lich";
                break;
            default:
                bossName = "Unknown_Boss";
        }
        /*
        auto boss = make_shared<Mob*>(bossName);

        int bossLevel        = rng.generateInRange(mobLevelMin, mobLevelMax);
        int healthMultiplier = isBossRoom ? 3 : 1;

        boss->SetMaxHealth(100 * bossLevel / 10 * healthMultiplier * difficultyMultiplier);
        boss->SetHealth(boss->GetMaxHealth());
        boss->SetMaxMana(50 * bossLevel / 10 * difficultyMultiplier);
        boss->SetCurrentMana(boss->GetMaxMana());
        boss->SetAttackDamage(10 * bossLevel / 10 * difficultyMultiplier);
        boss->SetLevel(bossLevel);

        mobs.push_back(boss);
        */
    }
    else
    {
        int numMobs = std::min(1 + chamberNumber / 2, 4);

        // Double chambers have more mobs
        if (isDoubleChamber)
        {
            numMobs = static_cast<int>(numMobs * 1.5);
        }

        for (int i = 0; i < numMobs; i++)
        {
            int    mobType = rng.generateInRange(4);
            string mobName;

            std::unique_ptr<Mob> mob;
            switch (mobType)
            {
                case 0:
                    mob = std::make_unique<Slime>(
                        "Slime", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                    break;
                case 1:
                    mob = std::make_unique<Goblin>(
                        "Goblin", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                    break;
                case 2:
                    mob = std::make_unique<Skeleton>("Skeleton",
                                                     sf::Vector2f(0, 0),
                                                     *gameContext.GetNavigationGrid(),
                                                     gameContext);
                    break;
                case 3:
                    mob = std::make_unique<Orc>(
                        "Orc", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                    break;
                default:
                    // it should never reach here
                    mob = std::make_unique<Slime>(
                        "Slime", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
            }

            // Set mob stats based on chamber difficulty
            int mobLevel = rng.generateInRange(mobLevelMin, mobLevelMax);

            mob->SetMaxHealth(60 * mobLevel / 10 * difficultyMultiplier);
            mob->SetHealth(mob->GetMaxHealth());
            mob->SetMaxMana(30 * mobLevel / 10 * difficultyMultiplier);
            mob->SetCurrentMana(mob->GetMaxMana());
            mob->SetAttackDamage(8 * mobLevel / 10 * difficultyMultiplier);
            mob->SetLevel(mobLevel);
            mob->SetActive(false);

            mobsId.push_back(mob->GetId());
            unitManager.AddUnit(std::move(mob));
        }
    }

    calculateRewards(isDoubleChamber);
}

void Chamber::calculateRewards(bool isDoubleChamber)
{
    goldReward = chamberNumber * 50 + (mobLevelMin + mobLevelMax) / 2 * 10;
    expReward  = chamberNumber * 100 + (mobLevelMin + mobLevelMax) / 2 * 20;

    goldReward = static_cast<int>(goldReward * difficultyMultiplier);
    expReward  = static_cast<int>(expReward * difficultyMultiplier);

    if (isBossRoom)
    {
        goldReward *= 2;
        expReward *= 2;
    }

    if (isDoubleChamber)
    {
        goldReward *= 3;
        expReward *= 3;

        this->isDoubleChamber = isDoubleChamber;
    }
}

void Chamber::addMob(unsigned int mobId)
{
    mobsId.push_back(mobId);
}

void Chamber::generateMobLoot(const MobLootConfigParser& lootConfigParser, ItemManager& itemManager)
{
    std::map<std::string, std::map<std::string, float>> mobLootData = lootConfigParser.GetData();

    vector<Item> allItems = itemManager.getAllItems();

    if (allItems.empty())
    {
        return;
    }

    for (const auto& mobId : mobsId)
    {
        Mob*   mob     = unitManager.GetUnitOfType<Mob>(mobId);
        string mobName = mob->GetName();

        auto mobIt = mobLootData.find(mobName);
        if (mobIt != mobLootData.end())
        {
            const auto& itemsMap = mobIt->second;

            for (const auto& itemEntry : itemsMap)
            {
                const string& itemId          = itemEntry.first;
                float         dropProbability = itemEntry.second;

                double randomProb = rng.generateProbability();

                if (randomProb <= dropProbability)
                {
                    try
                    {
                        Item& item = itemManager.getItem(itemId);

                        mobLoot.push_back(item);

                        cout << "Mob " << mobName << " dropped item " << item.getName()
                             << " (ID: " << itemId << ")" << endl;
                    }
                    catch (const char* error)
                    {
                        cerr << "Error retrieving item " << itemId << ": " << error << endl;
                    }
                }
            }
        }
    }
}

void Chamber::addLootItem(const Item& item)
{
    mobLoot.push_back(item);
}

vector<Item> Chamber::clearChamber()
{
    isCleared = true;

    return mobLoot;
}

vector<unsigned int> Chamber::getMobsId() const
{
    return mobsId;
}

vector<Item> Chamber::getMobLoot() const
{
    return mobLoot;
}

void Chamber::setCleared(bool cleared)
{
    isCleared = cleared;
}

bool Chamber::getIsCleared() const
{
    return isCleared;
}

double Chamber::getDifficultyMultiplier() const
{
    return difficultyMultiplier;
}

int Chamber::getChamberNumber() const
{
    return chamberNumber;
}

bool Chamber::getIsBossRoom() const
{
    return isBossRoom;
}

int Chamber::getMobLevelMin() const
{
    return mobLevelMin;
}

int Chamber::getMobLevelMax() const
{
    return mobLevelMax;
}

int Chamber::getGoldReward() const
{
    return goldReward;
}

int Chamber::getExpReward() const
{
    return expReward;
}

void Chamber::setGoldReward(int gold)
{
    goldReward = gold;
}

void Chamber::setExpReward(int exp)
{
    expReward = exp;
}

void Chamber::setIsDoubleChamber(bool isDouble)
{
    if (isDouble != isDoubleChamber)
    {
        isDoubleChamber = isDouble;

        calculateRewards(isDoubleChamber);

        if (isDoubleChamber)
        {
            mobLevelMin = static_cast<int>(mobLevelMin * 1.2);
            mobLevelMax = static_cast<int>(mobLevelMax * 1.2);
            difficultyMultiplier *= 2.0;
        }
    }
}

bool Chamber::getIsDoubleChamber() const
{
    return isDoubleChamber;
}