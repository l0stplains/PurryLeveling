#pragma once

#include <string>
#include <vector>

#include "dungeon/Chamber.hpp"
#include "items/Item.hpp"
#include "items/ItemManager.hpp"
#include "parser/MobLootConfigParser.hpp"
#include "quest/Quest.hpp"

using namespace std;

/**
 * @brief Dungeon class represents a dungeon with multiple chambers
 */
class Dungeon
{
private:
    string          rank;            // The rank of the dungeon (S, A, B, C, D, E, SPECIAL)
    vector<Chamber> chambers;        // Chambers in the dungeon
    int             entranceFee;     // Gold cost to enter the dungeon
    int             minPlayerLevel;  // Minimum player level required to enter
    int             playerLevel;     // Current player level
    int             initGold;        // Gold obtained from the dungeon
    int             initExp;         // Experience obtained from the dungeon
    bool            isCleared;       // Whether the dungeon has been cleared
    vector<Item>    dungeonLoot;  // Items obtained from completing the dungeon (completion reward)
    vector<Item>    mobLoot;      // Items obtained from mobs in cleared chambers
    double          expMultiplier;             // Experience multiplier for this dungeon
    double          goldMultiplier;            // Gold multiplier for this dungeon
    ItemManager&    itemManager;               // Pointer to the ItemManager
    MobLootConfigParser& mobLootConfigParser;  // Pointer to the MobLootConfigParser
    Quest                quest;                // Quest associated with this dungeon

    /**
     * @brief Initialize dungeon properties based on rank
     */
    void setupDungeonProperties();

    /**
     * @brief Set the player level
     *
     * @param level The player level
     */
    void setPlayerLevel(int level);

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
     * @brief Count the number of double chambers in the dungeon
     *
     * @return int The number of double chambers
     */
    int countDoubleChambers() const;

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
     * @brief Add an item to the dungeon completion loot
     *
     * @param item The item to add
     */
    void addLoot(const Item& item);

    /**
     * @brief Add an item to the mob loot collection
     *
     * @param item The item to add
     */
    void addMobLoot(const Item& item);

    /**
     * @brief Get the item reward ID from the quest
     * Only valid if quest is completed and dungeon is cleared
     *
     * @return std::string Quest item reward ID, or empty string if conditions not met
     */
    string getQuestItemRewardId() const;

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
     * @param itemManager Pointer to an ItemManager instance
     * @param mobLootConfigParser Pointer to a MobLootConfigParser instance
     * @param gold Initial gold value
     * @param exp Initial experience value
     */
    Dungeon(const string&        rank,
            int                  playerLevel,
            ItemManager&         itemManager,
            MobLootConfigParser& mobLootConfigParser,
            int                  gold,
            int                  exp);

    /**
     * @brief Destroy the Dungeon object
     */
    ~Dungeon();

    /**
     * @brief Add a chamber to the dungeon
     *
     * @param chamber The chamber to add
     */
    void addChamber(const Chamber& chamber);

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
     * @brief Clear a specific chamber and add its loot to the mob loot collection
     *
     * @param index The index of the chamber to clear
     */
    void clearChamber(int index);

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
     * @brief Get the dungeon completion loot
     *
     * @return vector<Item> The dungeon completion loot
     */
    vector<Item> getLoot() const;

    /**
     * @brief Get the mob loot collection
     *
     * @return vector<Item> The mob loot collection
     */
    vector<Item> getMobLoot() const;

    /**
     * @brief Generate completion loot based on dungeon rank
     * This uses the internal ItemManager
     */
    void generateLoot();

    /**
     * @brief Generate mob loot for all chambers
     * This uses the internal MobLootConfigParser and ItemManager
     */
    void generateMobLoot();

    /**
     * @brief Clear the dungeon loot
     */
    void clearLoot();

    /**
     * @brief Clear the mob loot
     */
    void clearMobLoot();

    /**
     * @brief Get the number of chambers in the dungeon
     *
     * @return int The number of chambers
     */
    int getNumChambers() const;

    /**
     * @brief Check if all chambers are cleared
     *
     * @return true If all chambers are cleared
     * @return false If not all chambers are cleared
     */
    bool areAllChambersCleared() const;

    /**
     * @brief Get penalties for failing to complete the dungeon
     *
     * @param expPenalty Reference to store experience penalty
     * @param goldPenalty Reference to store gold penalty
     */
    void getPenalty(int& expPenalty, int& goldPenalty) const;

    /**
     * @brief Get the initial gold value
     *
     * @return int The initial gold value
     */
    int getInitGold() const;

    /**
     * @brief Get the initial experience value
     *
     * @return int The initial experience value
     */
    int getInitExp() const;

    /**
     * @brief Get the rank of the dungeon
     *
     * @return string The rank of the dungeon
     */
    string getRank() const;

    /*
     * @brief Set the quest for this dungeon
     *
     * @param newQuest The quest to associate with this dungeon
     */
    void setQuest(const Quest& newQuest);

    /**
     * @brief Get the quest associated with this dungeon
     *
     * @return Quest& Reference to the quest
     */
    Quest& getQuest();

    /**
     * @brief Get the quest associated with this dungeon (const version)
     *
     * @return const Quest& Const reference to the quest
     */
    const Quest& getQuest() const;

    /**
     * @brief Check if the quest is completed and all chambers are cleared
     *
     * @return true If quest is completed and dungeon is cleared
     * @return false Otherwise
     */
    bool isQuestCompleted() const;

    /**
     * @brief Update quest progress for KILL type quests
     *
     * @param mobsKilled Number of mobs killed
     * @return true If the quest was completed with this update
     * @return false Otherwise
     */
    bool updateKillQuestProgress(int mobsKilled);

    /**
     * @brief Update quest progress for DAMAGE type quests
     *
     * @param damageDealt Amount of damage dealt
     * @return true If the quest was completed with this update
     * @return false Otherwise
     */
    bool updateDamageQuestProgress(int damageDealt);

    /**
     * @brief Get the gold reward from the quest
     * Only valid if quest is completed and dungeon is cleared
     *
     * @return int Quest gold reward, or 0 if conditions not met
     */
    int getQuestGoldReward() const;

    /**
     * @brief Get the experience reward from the quest
     * Only valid if quest is completed and dungeon is cleared
     *
     * @return int Quest experience reward, or 0 if conditions not met
     */
    int getQuestExpReward() const;

    /**
     * @brief Get the actual item reward from the quest
     * Only valid if quest is completed and dungeon is cleared
     *
     * @return Item The quest reward item, or a null item if conditions not met
     */
    Item getQuestRewardItem() const;
};