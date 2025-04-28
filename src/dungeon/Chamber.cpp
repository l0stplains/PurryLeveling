#include "dungeon/Chamber.hpp"

#include <iostream>

using namespace std;

Chamber::Chamber(int chamberNumber, bool isBossRoom, double difficultyMultiplier, int mobLevelMin, int mobLevelMax)
    : chamberNumber(chamberNumber),
      isBossRoom(isBossRoom),
      difficultyMultiplier(difficultyMultiplier),
      mobLevelMin(mobLevelMin),
      mobLevelMax(mobLevelMax),
      isCleared(false),
      goldReward(0),
      expReward(0),
      rng()
{
    // Generate mobs based on whether this is a boss room
    generateMobs(isBossRoom);
}

Chamber::~Chamber()
{
    // Clean up any resources if needed
}

void Chamber::generateMobs(bool isBossRoom)
{
    if (isBossRoom) {
        // Generate a boss mob - can be random or based on the chamber difficulty
        int bossType = rng.generateInRange(4); // 0-3 for different boss types
        
        // For simplicity, let's assign boss IDs in range 100-103
        int bossId = 100 + bossType;
        mobs.push_back(bossId);
        
        // Add potential boss loot with higher value items
        string bossLoot;
        switch (bossType) {
            case 0: bossLoot = "OgreAxe"; break;
            case 1: bossLoot = "DarkSword"; break;
            case 2: bossLoot = "DemonCrown"; break;
            case 3: bossLoot = "SoulStaff"; break;
        }
        
        addMobLoot(bossId, bossLoot);
    } else {
        // Number of mobs is based on chamber number (more mobs in later chambers)
        int numMobs = 1 + chamberNumber / 2;
        
        for (int i = 0; i < numMobs; i++) {
            // Randomly select a mob type (0-3 for different mob types)
            int mobType = rng.generateInRange(4);
            
            // For simplicity, let's assign regular mob IDs in range 0-3
            int mobId = mobType;
            mobs.push_back(mobId);
            
            // Add potential mob loot
            string mobLoot;
            switch (mobType) {
                case 0: mobLoot = "SlimeJelly"; break;
                case 1: mobLoot = "GoblinDagger"; break;
                case 2: mobLoot = "BoneFragment"; break;
                case 3: mobLoot = "OrcTusk"; break;
            }
            
            // Only add loot if RNG determines it should drop (50% chance)
            if (rng.generateProbability() < 0.5) {
                addMobLoot(mobId, mobLoot);
            }
        }
    }
    
    // Calculate rewards after generating mobs
    calculateRewards(false);
}

void Chamber::calculateRewards(bool isDoubleDungeon)
{
    // Base rewards are calculated based on chamber number and mob levels
    goldReward = chamberNumber * 50 + (mobLevelMin + mobLevelMax) / 2 * 10;
    expReward = chamberNumber * 100 + (mobLevelMin + mobLevelMax) / 2 * 20;
    
    // Apply difficulty multiplier
    goldReward = static_cast<int>(goldReward * difficultyMultiplier);
    expReward = static_cast<int>(expReward * difficultyMultiplier);
    
    // Boss rooms provide better rewards
    if (isBossRoom) {
        goldReward *= 2;
        expReward *= 2;
    }
    
    // Double dungeons provide even better rewards
    if (isDoubleDungeon) {
        goldReward *= 3;
        expReward *= 3;
    }
}

void Chamber::addMob(int mobId)
{
    mobs.push_back(mobId);
}

void Chamber::addMobLoot(int mobId, const string& lootItem)
{
    mobLoot[mobId] = lootItem;
}

void Chamber::addLoot(const string& item)
{
    loot.push_back(item);
}

vector<int> Chamber::getMobs() const
{
    return mobs;
}

vector<string> Chamber::getLoot() const
{
    return loot;
}

map<int, string> Chamber::getMobLoot() const
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
