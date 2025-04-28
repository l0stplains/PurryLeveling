#include "dungeon/DungeonFactory.hpp"
#include <iostream>

DungeonFactory::DungeonFactory()
    : rng(), isDoubleDungeonYet(false), doubleDungeonActivateThreshold(0.05), chance(0.0)
{}

DungeonFactory::~DungeonFactory() {}

int DungeonFactory::getNumChambers(const string& rank) const
{
    if (rank == Dungeon::RANK_S) return 6;
    if (rank == Dungeon::RANK_A) return 5;
    if (rank == Dungeon::RANK_B) return 4;
    if (rank == Dungeon::RANK_C) return 3;
    if (rank == Dungeon::RANK_D) return 2;
    if (rank == Dungeon::RANK_E) return 1;
    if (rank == Dungeon::RANK_SPECIAL) return 100;
    
    return 1;
}

void DungeonFactory::getMobLevelRange(const string& rank, int playerLevel, int& minLevel, int& maxLevel) const
{
    if (rank == Dungeon::RANK_S) {
        minLevel = 50;
        maxLevel = 100;
    } else if (rank == Dungeon::RANK_A) {
        minLevel = 30;
        maxLevel = 70;
    } else if (rank == Dungeon::RANK_B) {
        minLevel = 20;
        maxLevel = 50;
    } else if (rank == Dungeon::RANK_C) {
        minLevel = 10;
        maxLevel = 35;
    } else if (rank == Dungeon::RANK_D) {
        minLevel = 0;
        maxLevel = 25;
    } else if (rank == Dungeon::RANK_E) {
        minLevel = 0;
        maxLevel = 10;
    } else if (rank == Dungeon::RANK_SPECIAL) {
        minLevel = 0;
        maxLevel = playerLevel + 20;
    } else {
        minLevel = 0;
        maxLevel = 10;
    }
}

double DungeonFactory::getDifficultyMultiplier(const string& rank, int chamberIndex) const
{
    double difficultyMultiplier = 1.0;

    if (rank == Dungeon::RANK_S) {
        difficultyMultiplier = 5.0;
    } else if (rank == Dungeon::RANK_A) {
        difficultyMultiplier = 4.0;
    } else if (rank == Dungeon::RANK_B) {
        difficultyMultiplier = 3.0;
    } else if (rank == Dungeon::RANK_C) {
        difficultyMultiplier = 2.0;
    } else if (rank == Dungeon::RANK_D) {
        difficultyMultiplier = 1.5;
    } else if (rank == Dungeon::RANK_E) {
        difficultyMultiplier = 1.0;
    } else if (rank == Dungeon::RANK_SPECIAL) {
        difficultyMultiplier = 1.0 + (chamberIndex * 0.08);
    }

    return difficultyMultiplier;
}

bool DungeonFactory::isBossRoom(const string& rank, int chamberIndex, int totalChambers) const
{
    if (rank == Dungeon::RANK_SPECIAL && (chamberIndex + 1) % 10 == 0) {
        return true;
    }

    if (chamberIndex == totalChambers - 1) {
        return (rank != Dungeon::RANK_D && rank != Dungeon::RANK_E);
    }

    return false;
}

Dungeon DungeonFactory::createDungeon(const string& rank, int playerLevel)
{
    Dungeon dungeon(rank, playerLevel);
    dungeon.setIsDoubleDungeon(false);
    this->isDoubleDungeonYet = false;

    int numChambers = getNumChambers(rank);

    int mobLevelMin, mobLevelMax;
    getMobLevelRange(rank, playerLevel, mobLevelMin, mobLevelMax);

    for (int i = 0; i < numChambers; i++) {
        if (!isDoubleDungeonYet) {
            if (shouldBeDoubleDungeon()) {
                isDoubleDungeonYet = true;
                dungeon.setIsDoubleDungeon(true);
            }
        }

        bool isBossRoom = this->isBossRoom(rank, i, numChambers);

        double difficultyMultiplier = getDifficultyMultiplier(rank, i);

        if (isDoubleDungeonYet) {
            difficultyMultiplier *= 2.0;
        }

        int specialMinLevel = mobLevelMin;
        int specialMaxLevel = mobLevelMax;

        if (rank == Dungeon::RANK_SPECIAL) {
            int levelIncrease = i * 2;
            specialMinLevel += levelIncrease;
            specialMaxLevel += levelIncrease;
        }

        Chamber chamber(
            i + 1,
            isBossRoom,
            difficultyMultiplier,
            rank == Dungeon::RANK_SPECIAL ? specialMinLevel : mobLevelMin,
            rank == Dungeon::RANK_SPECIAL ? specialMaxLevel : mobLevelMax
        );

        dungeon.addChamber(chamber);
    }

    return dungeon;
}

bool DungeonFactory::shouldBeDoubleDungeon()
{
    chance = rng.generatePercentage();
    return (chance <= doubleDungeonActivateThreshold * 100);
}
