#ifndef CHAMBER_HPP
#define CHAMBER_HPP

#include <map>
#include <string>
#include <vector>

#include "rng/rng.hpp"

using namespace std;

/**
 * @brief Chamber class represents a single chamber within a dungeon
 * A chamber contains mobs and can have different difficulty levels
 */
class Chamber
{
private:
    int              chamberNumber;         // The sequence number of this chamber in the dungeon
    bool             isBossRoom;            // Whether this chamber contains a boss
    vector<int>      mobs;                  // IDs of mobs in this chamber
    vector<string>   loot;                  // Items obtained in this chamber
    map<int, string> mobLoot;               // Map of mob ID to loot item
    double           difficultyMultiplier;  // Multiplier for chamber difficulty
    int              mobLevelMin;           // Minimum level for mobs in this chamber
    int              mobLevelMax;           // Maximum level for mobs in this chamber
    bool             isCleared;             // Whether this chamber has been cleared
    int              goldReward;            // Gold reward for clearing this chamber
    int              expReward;             // Experience reward for clearing this chamber
    RNG              rng;                   // Random number generator

public:
    /**
     * @brief Construct a new Chamber object
     *
     * @param chamberNumber The sequence number of this chamber
     * @param isBossRoom Whether this chamber contains a boss
     * @param difficultyMultiplier Multiplier for chamber difficulty
     * @param mobLevelMin Minimum level for mobs
     * @param mobLevelMax Maximum level for mobs
     */
    Chamber(int    chamberNumber,
            bool   isBossRoom,
            double difficultyMultiplier,
            int    mobLevelMin,
            int    mobLevelMax);

    /**
     * @brief Destroy the Chamber object
     */
    ~Chamber();

    /**
     * @brief Generate mobs for this chamber
     *
     * @param isBossRoom Whether to generate boss mobs
     */
    void generateMobs(bool isBossRoom);

    /**
     * @brief Calculate rewards for this chamber
     *
     * @param isDoubleDungeon Whether this is a double dungeon
     */
    void calculateRewards(bool isDoubleDungeon);

    /**
     * @brief Add a mob to this chamber
     *
     * @param mobId ID of the mob to add
     */
    void addMob(int mobId);

    /**
     * @brief Add mob loot mapping
     *
     * @param mobId ID of the mob
     * @param lootItem Loot item from this mob
     */
    void addMobLoot(int mobId, const string& lootItem);

    /**
     * @brief Add an item to the chamber's loot
     *
     * @param item Item to add to loot
     */
    void addLoot(const string& item);

    /**
     * @brief Get the list of mobs in this chamber
     *
     * @return vector<int> Vector of mob IDs
     */
    vector<int> getMobs() const;

    /**
     * @brief Get the list of loot in this chamber
     *
     * @return vector<string> Vector of loot items
     */
    vector<string> getLoot() const;

    /**
     * @brief Get the mob loot mappings
     *
     * @return map<int, string> Map of mob ID to loot item
     */
    map<int, string> getMobLoot() const;

    /**
     * @brief Set the chamber as cleared
     *
     * @param cleared Whether the chamber is cleared
     */
    void setCleared(bool cleared);

    /**
     * @brief Check if the chamber is cleared
     *
     * @return true If the chamber is cleared
     * @return false If the chamber is not cleared
     */
    bool getIsCleared() const;

    /**
     * @brief Get the difficulty multiplier of the chamber
     *
     * @return double The difficulty multiplier
     */
    double getDifficultyMultiplier() const;

    /**
     * @brief Get the chamber number
     *
     * @return int The chamber number
     */
    int getChamberNumber() const;

    /**
     * @brief Check if this is a boss room
     *
     * @return true If this is a boss room
     * @return false If this is not a boss room
     */
    bool getIsBossRoom() const;

    /**
     * @brief Get the minimum mob level
     *
     * @return int The minimum mob level
     */
    int getMobLevelMin() const;

    /**
     * @brief Get the maximum mob level
     *
     * @return int The maximum mob level
     */
    int getMobLevelMax() const;

    /**
     * @brief Get the gold reward for clearing this chamber
     *
     * @return int The gold reward
     */
    int getGoldReward() const;

    /**
     * @brief Get the experience reward for clearing this chamber
     *
     * @return int The experience reward
     */
    int getExpReward() const;

    /**
     * @brief Set the gold reward for clearing this chamber
     *
     * @param gold The gold reward
     */
    void setGoldReward(int gold);

    /**
     * @brief Set the experience reward for clearing this chamber
     *
     * @param exp The experience reward
     */
    void setExpReward(int exp);
};

#endif
