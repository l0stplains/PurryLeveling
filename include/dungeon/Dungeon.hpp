#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <string>
#include <vector>

#include "dungeon/Chamber.hpp"

using namespace std;

/**
 * @brief Dungeon class represents a dungeon with multiple chambers
 */
class Dungeon
{
private:
    string              rank;                // The rank of the dungeon (S, A, B, C, D, E, SPECIAL)
    vector<Chamber>     chambers;           // Chambers in the dungeon
    bool                isDoubleDungeon;    // Whether this is a double dungeon
    int                 entranceFee;        // Gold cost to enter the dungeon
    int                 minPlayerLevel;     // Minimum player level required to enter
    int                 playerLevel;        // Current player level
    bool                isCleared;          // Whether the dungeon has been cleared
    vector<string>      dungeonLoot;        // Items obtained from the dungeon
    double              expMultiplier;      // Experience multiplier for this dungeon
    double              goldMultiplier;     // Gold multiplier for this dungeon

public:
    // Constants for dungeon ranks
    static const string RANK_S;
    static const string RANK_A;
    static const string RANK_B;
    static const string RANK_C;
    static const string RANK_D;
    static const string RANK_E;
    static const string RANK_SPECIAL;

    /**
     * @brief Construct a new Dungeon object
     *
     * @param rank The rank of the dungeon
     * @param playerLevel The level of the player
     */
    Dungeon(const string& rank, int playerLevel);

    /**
     * @brief Destroy the Dungeon object
     */
    ~Dungeon();

    /**
     * @brief Initialize dungeon properties based on rank
     */
    void setupDungeonProperties();

    /**
     * @brief Add a chamber to the dungeon
     *
     * @param chamber The chamber to add
     */
    void addChamber(const Chamber& chamber);

    /**
     * @brief Set whether this is a double dungeon
     *
     * @param isDouble Whether this is a double dungeon
     */
    void setIsDoubleDungeon(bool isDouble);

    /**
     * @brief Check if this is a double dungeon
     *
     * @return true If this is a double dungeon
     * @return false If this is not a double dungeon
     */
    bool getIsDoubleDungeon() const;

    /**
     * @brief Set the player level
     *
     * @param level The player level
     */
    void setPlayerLevel(int level);

    /**
     * @brief Get the player level
     *
     * @return int The player level
     */
    int getPlayerLevel() const;

    /**
     * @brief Get the entrance fee
     *
     * @return int The entrance fee
     */
    int getEntranceFee() const;

    /**
     * @brief Get the minimum player level
     *
     * @return int The minimum player level
     */
    int getMinPlayerLevel() const;

    /**
     * @brief Get the rank of the dungeon
     *
     * @return string The rank of the dungeon
     */
    string getRank() const;

    /**
     * @brief Get the chambers in the dungeon
     *
     * @return vector<Chamber> The chambers in the dungeon
     */
    vector<Chamber> getChambers() const;

    /**
     * @brief Get a specific chamber
     *
     * @param index The index of the chamber
     * @return Chamber& Reference to the chamber
     */
    Chamber& getChamber(int index);

    /**
     * @brief Set whether the dungeon is cleared
     *
     * @param cleared Whether the dungeon is cleared
     */
    void setCleared(bool cleared);

    /**
     * @brief Check if the dungeon is cleared
     *
     * @return true If the dungeon is cleared
     * @return false If the dungeon is not cleared
     */
    bool getIsCleared() const;

    /**
     * @brief Add an item to the dungeon loot
     *
     * @param item The item to add
     */
    void addLoot(const string& item);

    /**
     * @brief Get the dungeon loot
     *
     * @return vector<string> The dungeon loot
     */
    vector<string> getLoot() const;

    /**
     * @brief Clear the dungeon loot
     */
    void clearLoot();

    /**
     * @brief Get the number of chambers in the dungeon
     *
     * @return int The number of chambers
     */
    int getNumChambers() const;

    /**
     * @brief Get the experience multiplier
     *
     * @return double The experience multiplier
     */
    double getExpMultiplier() const;

    /**
     * @brief Get the gold multiplier
     *
     * @return double The gold multiplier
     */
    double getGoldMultiplier() const;

    /**
     * @brief Set the experience multiplier
     *
     * @param multiplier The experience multiplier
     */
    void setExpMultiplier(double multiplier);

    /**
     * @brief Set the gold multiplier
     *
     * @param multiplier The gold multiplier
     */
    void setGoldMultiplier(double multiplier);

    /**
     * @brief Check if all chambers are cleared
     *
     * @return true If all chambers are cleared
     * @return false If not all chambers are cleared
     */
    bool areAllChambersCleared() const;
};

#endif
