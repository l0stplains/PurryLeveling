#pragma once

#include <map>
#include <memory>  // For shared_ptr
#include <string>
#include <vector>

#include "core/GameContext.hpp"
#include "core/UnitManager.hpp"

#include "items/Item.hpp"
#include "items/ItemManager.hpp"
#include "parser/MobLootConfigParser.hpp"
#include "rng/rng.hpp"
#include "units/Unit.hpp"
#include "units/mobs/Mob.hpp"

using namespace std;

/**
 * @brief Chamber class represents a single chamber within a dungeon
 * A chamber contains mobs and can have different difficulty levels
 */
class Chamber
{
private:
    int  chamberNumber;    // The sequence number of this chamber in the dungeon [1 indexed]
    bool isBossRoom;       // Whether this chamber contains a boss
    bool isDoubleChamber;  // Whether this is a double chamber (increased difficulty and rewards)
    vector<unsigned int> mobsId;   // Mobs in this chamber (using shared_ptr for polymorphism)
    vector<Item>         mobLoot;  // Loot items dropped by mobs in this chamber
    double               difficultyMultiplier;  // Multiplier for chamber difficulty
    int                  mobLevelMin;           // Minimum level for mobs in this chamber
    int                  mobLevelMax;           // Maximum level for mobs in this chamber
    bool                 isCleared;             // Whether this chamber has been cleared
    int                  goldReward;            // Gold reward for clearing this chamber
    int                  expReward;             // Experience reward for clearing this chamber
    RNG                  rng;                   // Random number generator
    UnitManager&         unitManager;           // Reference to the UnitManager for unit generation
    GameContext&         gameContext;  // Reference to the GameContext for game state management

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

    /**
     * @brief Calculate rewards for this chamber
     *
     * @param isDoubleChamber Whether this is a double chamber
     */
    void calculateRewards(bool isDoubleChamber);

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
     * @brief Add a mob to this chamber
     *
     * @param mob The mob to add
     */
    void addMob(unsigned int mobId);

    /**
     * @brief Add a loot item to this chamber
     *
     * @param item The item to add as loot
     */
    void addLootItem(const Item& item);

    /**
     * @brief Get the difficulty multiplier of the chamber
     *
     * @return double The difficulty multiplier
     */
    double getDifficultyMultiplier() const;

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
    Chamber(int          chamberNumber,
            bool         isBossRoom,
            double       difficultyMultiplier,
            int          mobLevelMin,
            int          mobLevelMax,
            UnitManager& unitManager,
            GameContext& gameContext);

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
     * @brief Get the list of mobs in this chamber
     *
     * @return vector<unsigned int> Vector of mobs id
     */
    vector<unsigned int> getMobsId() const;

    /**
     * @brief Generate loot for mobs in this chamber
     *
     * @param lootConfigParser The mob loot configuration parser
     * @param itemManager The item manager for retrieving items
     */
    void generateMobLoot(const MobLootConfigParser& lootConfigParser, ItemManager& itemManager);

    /**
     * @brief Get the mob loot items
     *
     * @return vector<Item> Vector of loot items
     */
    vector<Item> getMobLoot() const;

    /**
     * @brief Mark this chamber as cleared and return its loot
     *
     * @return vector<Item> The loot items from this chamber
     */
    vector<Item> clearChamber();

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
     * @brief Get the chamber number
     *
     * @return int The chamber number
     */
    int getChamberNumber() const;

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
     * @brief Check if this is a double chamber
     *
     * @return true If this is a double chamber
     * @return false If this is not a double chamber
     */
    bool getIsDoubleChamber() const;

    /**
     * @brief Set whether this is a double chamber
     *
     * @param isDouble Whether this is a double chamber
     */
    void setIsDoubleChamber(bool isDouble);
};