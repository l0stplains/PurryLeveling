#include "dungeon/Chamber.hpp"

#include <cmath>
#include <iostream>
#include <map>

#include "units/mobs/basics/Goblin.hpp"
#include "units/mobs/basics/Orc.hpp"
#include "units/mobs/basics/Skeleton.hpp"
#include "units/mobs/basics/Slime.hpp"
#include "units/mobs/bosses/DarkKnight.hpp"
#include "units/mobs/bosses/DemonLord.hpp"
#include "units/mobs/bosses/Lich.hpp"
#include "units/mobs/bosses/Ogre.hpp"

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

        std::unique_ptr<Mob> bossMob;
        switch (bossType)
        {
            case 0:
                bossMob = std::make_unique<Ogre>(
                    "Ogre", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                break;
            case 1:
                bossMob = std::make_unique<DarkKnight>(
                    "Dark Knight", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                break;
            case 2:
                bossMob = std::make_unique<DemonLord>(
                    "Demon Lord", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                break;
            case 3:
                bossMob = std::make_unique<Lich>(
                    "Lich", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
                break;
            default:
                bossMob = std::make_unique<Ogre>(
                    "Ogre", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
        }
        int bossLevel        = rng.generateInRange(mobLevelMin, mobLevelMax);
        int healthMultiplier = 2;

        bossMob->SetMaxHealth(100 * bossLevel / 8 * healthMultiplier * difficultyMultiplier);
        bossMob->SetHealth(bossMob->GetMaxHealth());
        bossMob->SetMaxMana(50 * bossLevel / 10 * difficultyMultiplier);
        bossMob->SetCurrentMana(bossMob->GetMaxMana());
        bossMob->SetAttackDamage(15 * bossLevel / 8 * difficultyMultiplier);
        bossMob->SetLevel(bossLevel);

        bossMob->SetLevel(bossLevel);
        bossMob->SetActive(false);

        mobsId.push_back(bossMob->GetId());
        unitManager.AddUnit(std::move(bossMob));
    }
    else
    {
        int numMobs = std::min(1 + chamberNumber / 2, 4);

        if (isDoubleChamber)
        {
            numMobs = static_cast<int>(numMobs * 1.5);
        }

        for (int i = 0; i < numMobs; i++)
        {
            int mobType = rng.generateInRange(4);

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
                    mob = std::make_unique<Slime>(
                        "Slime", sf::Vector2f(0, 0), *gameContext.GetNavigationGrid(), gameContext);
            }

            int mobLevel = rng.generateInRange(mobLevelMin, mobLevelMax);

            mob->SetMaxHealth(60 * mobLevel / 8 * difficultyMultiplier);
            mob->SetHealth(mob->GetMaxHealth());
            mob->SetMaxMana(30 * mobLevel / 10 * difficultyMultiplier);
            mob->SetCurrentMana(mob->GetMaxMana());
            mob->SetAttackDamage(12 * mobLevel / 8 * difficultyMultiplier);
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
    goldReward = chamberNumber * 50 + std::pow((mobLevelMin + mobLevelMax) / 2, 1.2) * 1 + 10;
    expReward  = chamberNumber * 100 + std::pow((mobLevelMin + mobLevelMax) / 2, 1.2) * 2 + 100;

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
    const auto& mobLootData = lootConfigParser.GetData();

    vector<Item> allItems = itemManager.getAllItems();

    if (allItems.empty())
    {
        return;
    }

    static int specialDungeonTotalDrops = 0;
    const int MAX_SPECIAL_DUNGEON_DROPS = 5;
    bool isSpecialDungeon = (chamberNumber > 7);

    if (isSpecialDungeon && specialDungeonTotalDrops >= MAX_SPECIAL_DUNGEON_DROPS)
    {
        return;
    }

    bool directDropSpecial = isSpecialDungeon && chamberNumber >= 7;

    for (const auto& mobId : mobsId)
    {
        if (isSpecialDungeon && specialDungeonTotalDrops >= MAX_SPECIAL_DUNGEON_DROPS)
        {
            break;
        }
        
        Mob* mob = unitManager.GetUnitOfType<Mob>(mobId);
        string mobName = mob->GetName();

        auto mobIt = mobLootData.find(mobName);
        if (mobIt != mobLootData.end())
        {
            const auto& itemsMap = mobIt->second;

            if (directDropSpecial)
            {
                if (!itemsMap.empty())
                {
                    const auto& firstItem = *itemsMap.begin();
                    const string& itemId = firstItem.first;
                    
                    try
                    {
                        Item& item = itemManager.getItem(itemId);
                        mobLoot.push_back(item);
                        specialDungeonTotalDrops++;
                        
                        cout << "Mob " << mobName << " dropped item " << item.getName()
                             << " (ID: " << itemId << ")" << endl;

                        if (specialDungeonTotalDrops >= MAX_SPECIAL_DUNGEON_DROPS)
                        {
                            cout << "Special dungeon item drop cap reached (" << MAX_SPECIAL_DUNGEON_DROPS << " items)" << endl;
                            break;
                        }
                    }
                    catch (const ItemNotFoundException& e)
                    {
                        cout << "Item ID not found in mob loot: " << itemId
                             << ". Error: " << e.what() << endl;
                    }
                }
                continue;
            }

            for (const auto& itemEntry : itemsMap)
            {
                const string& itemId = itemEntry.first;
                float dropProbability = itemEntry.second;

                double randomProb = rng.generateProbability();

                if (randomProb <= dropProbability)
                {
                    try
                    {
                        Item& item = itemManager.getItem(itemId);
                        mobLoot.push_back(item);
                        
                        if (isSpecialDungeon)
                        {
                            specialDungeonTotalDrops++;
                        }

                        cout << "Mob " << mobName << " dropped item " << item.getName()
                             << " (ID: " << itemId << ")" << endl;

                        if (isSpecialDungeon)
                        {
                            if (specialDungeonTotalDrops >= MAX_SPECIAL_DUNGEON_DROPS)
                            {
                                cout << "Special dungeon item drop cap reached (" << MAX_SPECIAL_DUNGEON_DROPS << " items)" << endl;
                                break;
                            }
                            break;
                        }
                    }
                    catch (const ItemNotFoundException& e)
                    {
                        cout << "Item ID not found in mob loot: " << itemId
                             << ". Error: " << e.what() << endl;
                        continue;
                    }
                }
            }

            if (isSpecialDungeon && specialDungeonTotalDrops >= MAX_SPECIAL_DUNGEON_DROPS)
            {
                break;
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