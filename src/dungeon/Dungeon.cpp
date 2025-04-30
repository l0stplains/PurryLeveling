#include "dungeon/Dungeon.hpp"

const string Dungeon::RANK_S = "S";
const string Dungeon::RANK_A = "A";
const string Dungeon::RANK_B = "B";
const string Dungeon::RANK_C = "C";
const string Dungeon::RANK_D = "D";
const string Dungeon::RANK_E = "E";
const string Dungeon::RANK_SPECIAL = "SPECIAL";

Dungeon::Dungeon(const string& rank, int playerLevel)
    : rank(rank),
      isDoubleDungeon(false),
      entranceFee(0),
      minPlayerLevel(0),
      playerLevel(playerLevel),
      isCleared(false),
      expMultiplier(1.0),
      goldMultiplier(1.0)
{
    setupDungeonProperties();
}

Dungeon::~Dungeon() {}

void Dungeon::setupDungeonProperties()
{
    if (rank == RANK_S) {
        entranceFee = 1000;
        minPlayerLevel = 50;
        expMultiplier = 5.0;
        goldMultiplier = 5.0;
    } else if (rank == RANK_A) {
        entranceFee = 500;
        minPlayerLevel = 35;
        expMultiplier = 4.0;
        goldMultiplier = 4.0;
    } else if (rank == RANK_B) {
        entranceFee = 300;
        minPlayerLevel = 25;
        expMultiplier = 3.0;
        goldMultiplier = 3.0;
    } else if (rank == RANK_C) {
        entranceFee = 150;
        minPlayerLevel = 20;
        expMultiplier = 2.0;
        goldMultiplier = 2.0;
    } else if (rank == RANK_D) {
        entranceFee = 0;
        minPlayerLevel = 10;
        expMultiplier = 1.5;
        goldMultiplier = 1.5;
    } else if (rank == RANK_E) {
        entranceFee = 0;
        minPlayerLevel = 0;
        expMultiplier = 1.0;
        goldMultiplier = 1.0;
    } else if (rank == RANK_SPECIAL) {
        entranceFee = 0;
        minPlayerLevel = 0;
        expMultiplier = 2.0;
        goldMultiplier = 2.0;
    }
}

void Dungeon::addChamber(const Chamber& chamber)
{
    chambers.push_back(chamber);
}

void Dungeon::setIsDoubleDungeon(bool isDouble)
{
    if (!isDoubleDungeon && isDouble) {
        expMultiplier *= 3.0;
        goldMultiplier *= 3.0;
        
        for (Chamber& chamber : chambers) {
            chamber.calculateRewards(true);
        }
    }
    isDoubleDungeon = isDouble;
}

bool Dungeon::getIsDoubleDungeon() const
{
    return isDoubleDungeon;
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

void Dungeon::setCleared(bool cleared)
{
    isCleared = cleared;
}

bool Dungeon::getIsCleared() const
{
    return isCleared;
}

void Dungeon::addLoot(const string& item)
{
    dungeonLoot.push_back(item);
}

vector<string> Dungeon::getLoot() const
{
    return dungeonLoot;
}

void Dungeon::clearLoot()
{
    dungeonLoot.clear();
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
    for (const Chamber& chamber : chambers) {
        if (!chamber.getIsCleared()) {
            return false;
        }
    }
    return true;
}
