#include "dungeon/DungeonFactory.hpp"

#include "rng/rng.hpp"

DungeonFactory::DungeonFactory(ItemManager&         itemManager,
                               MobLootConfigParser& mobLootConfigParser,
                               QuestGenerator&      questGenerator)
    : rng(),
      itemManager(itemManager),
      mobLootConfigParser(mobLootConfigParser),
      questGenerator(questGenerator),
      doubleDungeonActivateThreshold(0.05),
      isDoubleDungeonYet(false),
      chance(0.0)
{}

DungeonFactory::~DungeonFactory() {}

string DungeonFactory::toLowercase(string str) const
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return result;
}

string DungeonFactory::normalizeRank(string rank) const
{
    string lowercaseRank = toLowercase(rank);

    if (lowercaseRank == "s")
        return Dungeon::RANK_S;
    if (lowercaseRank == "a")
        return Dungeon::RANK_A;
    if (lowercaseRank == "b")
        return Dungeon::RANK_B;
    if (lowercaseRank == "c")
        return Dungeon::RANK_C;
    if (lowercaseRank == "d")
        return Dungeon::RANK_D;
    if (lowercaseRank == "e")
        return Dungeon::RANK_E;
    if (lowercaseRank == "special")
        return Dungeon::RANK_SPECIAL;

    return rank;
}

int DungeonFactory::getNumChambers(const string& rank) const
{
    string normalizedRank = normalizeRank(rank);

    if (normalizedRank == Dungeon::RANK_S)
        return 6;
    if (normalizedRank == Dungeon::RANK_A)
        return 5;
    if (normalizedRank == Dungeon::RANK_B)
        return 4;
    if (normalizedRank == Dungeon::RANK_C)
        return 3;
    if (normalizedRank == Dungeon::RANK_D)
        return 2;
    if (normalizedRank == Dungeon::RANK_E)
        return 1;
    if (normalizedRank == Dungeon::RANK_SPECIAL)
        return 100;

    return 1;
}

void DungeonFactory::getMobLevelRange(const string& rank,
                                      int           playerLevel,
                                      int&          minLevel,
                                      int&          maxLevel) const
{
    string normalizedRank = normalizeRank(rank);

    if (normalizedRank == Dungeon::RANK_S)
    {
        minLevel = 50;
        maxLevel = 100;
    }
    else if (normalizedRank == Dungeon::RANK_A)
    {
        minLevel = 30;
        maxLevel = 70;
    }
    else if (normalizedRank == Dungeon::RANK_B)
    {
        minLevel = 20;
        maxLevel = 50;
    }
    else if (normalizedRank == Dungeon::RANK_C)
    {
        minLevel = 10;
        maxLevel = 35;
    }
    else if (normalizedRank == Dungeon::RANK_D)
    {
        minLevel = 0;
        maxLevel = 25;
    }
    else if (normalizedRank == Dungeon::RANK_E)
    {
        minLevel = 0;
        maxLevel = 10;
    }
    else if (normalizedRank == Dungeon::RANK_SPECIAL)
    {
        minLevel = 0;
        maxLevel = playerLevel + 20;
    }
    else
    {
        minLevel = 0;
        maxLevel = 10;
    }
}

double DungeonFactory::getDifficultyMultiplier(const string& rank, int chamberIndex) const
{
    string normalizedRank       = normalizeRank(rank);
    double difficultyMultiplier = 1.0;

    if (normalizedRank == Dungeon::RANK_S)
    {
        difficultyMultiplier = 5.0;
    }
    else if (normalizedRank == Dungeon::RANK_A)
    {
        difficultyMultiplier = 4.0;
    }
    else if (normalizedRank == Dungeon::RANK_B)
    {
        difficultyMultiplier = 3.0;
    }
    else if (normalizedRank == Dungeon::RANK_C)
    {
        difficultyMultiplier = 2.0;
    }
    else if (normalizedRank == Dungeon::RANK_D)
    {
        difficultyMultiplier = 1.5;
    }
    else if (normalizedRank == Dungeon::RANK_E)
    {
        difficultyMultiplier = 1.0;
    }
    else if (normalizedRank == Dungeon::RANK_SPECIAL)
    {
        difficultyMultiplier = 1.0 + (chamberIndex * 0.08);
    }

    return difficultyMultiplier;
}

bool DungeonFactory::isBossRoom(const string& rank, int chamberIndex, int totalChambers) const
{
    string normalizedRank = normalizeRank(rank);

    if (normalizedRank == Dungeon::RANK_SPECIAL && (chamberIndex + 1) % 10 == 0)
    {
        return true;
    }

    if (chamberIndex == totalChambers - 1)
    {
        return (normalizedRank != Dungeon::RANK_D && normalizedRank != Dungeon::RANK_E);
    }

    return false;
}

Dungeon DungeonFactory::createDungeon(const string& rank, int playerLevel, int playerGold, int playerExp)
{
    string  normalizedRank = normalizeRank(rank);
    Dungeon dungeon(
        normalizedRank, playerLevel, itemManager, mobLootConfigParser, playerGold, playerExp);

    int numChambers = getNumChambers(normalizedRank);

    int mobLevelMin, mobLevelMax;
    getMobLevelRange(normalizedRank, playerLevel, mobLevelMin, mobLevelMax);

    for (int i = 0; i < numChambers; i++)
    {
        bool   isBossRoom           = this->isBossRoom(normalizedRank, i, numChambers);
        double difficultyMultiplier = getDifficultyMultiplier(normalizedRank, i);

        int specialMinLevel = mobLevelMin;
        int specialMaxLevel = mobLevelMax;

        if (normalizedRank == Dungeon::RANK_SPECIAL)
        {
            int levelIncrease = i * 1.2;
            specialMinLevel += levelIncrease;
            specialMaxLevel += levelIncrease;
        }

        Chamber chamber(i + 1,
                        isBossRoom,
                        difficultyMultiplier,
                        normalizedRank == Dungeon::RANK_SPECIAL ? specialMinLevel : mobLevelMin,
                        normalizedRank == Dungeon::RANK_SPECIAL ? specialMaxLevel : mobLevelMax);

        bool isDoubleChamber = shouldBeDoubleChamber();
        if (isDoubleChamber)
        {
            chamber.setIsDoubleChamber(true);
        }

        chamber.generateMobs(isBossRoom);

        dungeon.addChamber(chamber);
    }

    dungeon.generateLoot();
    dungeon.generateMobLoot();

    if (hasQuestsForRank(rank))
    {
        Quest quest = questGenerator.generateQuest(rank);
        dungeon.setQuest(quest);
    }

    return dungeon;
}

bool DungeonFactory::shouldBeDoubleChamber()
{
    chance = rng.generatePercentage();
    return (chance <= doubleDungeonActivateThreshold * 100);
}

bool DungeonFactory::hasQuestsForRank(const string& rank) const
{
    return questGenerator.hasQuestsForRank(rank);
}