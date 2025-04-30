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
    generateMobs(isBossRoom);
}

Chamber::~Chamber()
{
}

void Chamber::generateMobs(bool isBossRoom)
{
    if (isBossRoom) {
        int bossType = rng.generateInRange(4);
        
        int bossId = 100 + bossType;
        mobs.push_back(bossId);

        string bossLoot;
        switch (bossType) {
            case 0: bossLoot = "OgreAxe"; break;
            case 1: bossLoot = "DarkSword"; break;
            case 2: bossLoot = "DemonCrown"; break;
            case 3: bossLoot = "SoulStaff"; break;
        }
        
        addMobLoot(bossId, bossLoot);
    } else {
        int numMobs = 1 + chamberNumber / 2;
        
        for (int i = 0; i < numMobs; i++) {
            int mobType = rng.generateInRange(4);
            
            int mobId = mobType;
            mobs.push_back(mobId);
            
            string mobLoot;
            switch (mobType) {
                case 0: mobLoot = "SlimeJelly"; break;
                case 1: mobLoot = "GoblinDagger"; break;
                case 2: mobLoot = "BoneFragment"; break;
                case 3: mobLoot = "OrcTusk"; break;
            }

            if (rng.generateProbability() < 0.5) {
                addMobLoot(mobId, mobLoot);
            }
        }
    }

    calculateRewards(false);
}

void Chamber::calculateRewards(bool isDoubleDungeon)
{
    goldReward = chamberNumber * 50 + (mobLevelMin + mobLevelMax) / 2 * 10;
    expReward = chamberNumber * 100 + (mobLevelMin + mobLevelMax) / 2 * 20;
    
    goldReward = static_cast<int>(goldReward * difficultyMultiplier);
    expReward = static_cast<int>(expReward * difficultyMultiplier);

    if (isBossRoom) {
        goldReward *= 2;
        expReward *= 2;
    }

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
