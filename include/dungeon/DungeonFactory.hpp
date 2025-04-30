#ifndef DUNGEON_FACTORY_HPP
#define DUNGEON_FACTORY_HPP

// Include
#include <algorithm>
#include <string>

#include "dungeon/Chamber.hpp"
#include "dungeon/Dungeon.hpp"
#include "rng/rng.hpp"

using namespace std;

/**
 * @brief Factory class responsible for creating Dungeon objects
 *
 * This class handles the creation of Dungeons with appropriate chambers
 * and properties based on the dungeon rank and player level.
 */
class DungeonFactory
{
private:
    RNG          rng;                             // Random number generator for dungeon creation
    const double doubleDungeonActivateThreshold;  // 5% chance of activating double dungeon
    bool         isDoubleDungeonYet;              // Tracks if double dungeon has been activated
    double       chance;                          // Stores the result of RNG for probability checks

    // Helper methods to break down the dungeon creation process
    /**
     * @brief Determines the number of chambers based on dungeon rank
     * @param rank The rank of the dungeon
     * @return Number of chambers for the dungeon
     */
    int getNumChambers(const string& rank) const;

    /**
     * @brief Gets the minimum and maximum mob levels for a given rank
     * @param rank The rank of the dungeon
     * @param playerLevel The level of the player
     * @param minLevel Output parameter for minimum mob level
     * @param maxLevel Output parameter for maximum mob level
     */
    void getMobLevelRange(const string& rank, int playerLevel, int& minLevel, int& maxLevel) const;

    /**
     * @brief Calculates the difficulty multiplier based on rank and chamber index
     * @param rank The rank of the dungeon
     * @param chamberIndex The index of the chamber (0-based)
     * @return The difficulty multiplier for the chamber
     */
    double getDifficultyMultiplier(const string& rank, int chamberIndex) const;

    /**
     * @brief Determines if a chamber should be a boss room
     * @param rank The rank of the dungeon
     * @param chamberIndex The index of the chamber (0-based)
     * @param totalChambers The total number of chambers in the dungeon
     * @return True if the chamber should be a boss room, false otherwise
     */
    bool isBossRoom(const string& rank, int chamberIndex, int totalChambers) const;

    /**
     * @brief Converts a string to lowercase
     * @param str The input string to convert
     * @return The lowercase version of the input string
     */
    string toLowercase(string str) const;

    /**
     * @brief Normalizes a rank string to the standard format
     *
     * Converts inputs like "a", "A", "S", "s", "special", "SPECIAL" to proper
     * formats used internally by the Dungeon class (e.g., "A", "S", "SPECIAL")
     *
     * @param rank The input rank string
     * @return The normalized rank string
     */
    string normalizeRank(string rank) const;

public:
    /**
     * @brief Constructs a new DungeonFactory object
     * Initializes the random number generator and internal state
     */
    DungeonFactory();

    /**
     * @brief Destroys the DungeonFactory object
     */
    ~DungeonFactory();

    /**
     * @brief Creates a new Dungeon with appropriate chambers and properties
     *
     * This is the main factory method that generates a complete dungeon
     * with the correct number of chambers, mob levels, and difficulty
     * based on the dungeon rank and player level.
     *
     * @param rank The rank of the dungeon as a string ("S", "A", "B", etc.)
     * @param playerLevel The level of the player
     * @return A fully configured Dungeon object
     */
    Dungeon createDungeon(const string& rank, int playerLevel);

    /**
     * @brief Determines if a dungeon should be a double dungeon
     *
     * Double dungeons have increased difficulty but provide greater rewards.
     *
     * @return True if the dungeon should be a double dungeon, false otherwise
     */
    bool shouldBeDoubleDungeon();
};

#endif