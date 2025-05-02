#pragma once

#include <string>

#include "items/Item.hpp"

using namespace std;

/**
 * @brief Quest types enumeration
 */
enum class QuestType
{
    KILL,   // Kill a specific number of mobs
    DAMAGE  // Deal a specific amount of damage
};

/**
 * @brief Quest class representing quests that players can complete in dungeons
 */
class Quest
{
private:
    QuestType type;             // Type of quest (KILL or DAMAGE)
    string    dungeonRank;      // Rank of dungeon where this quest is available
    int       targetCount;      // Required kills or damage to complete
    int       currentProgress;  // Current kills or damage accumulated
    int       goldReward;       // Gold reward for completing the quest
    int       expReward;        // Experience reward for completing the quest
    string    itemRewardId;     // ID of the item reward
    bool      isCompleted;      // Whether the quest is completed

public:
    /**
     * @brief Construct a new Quest object
     *
     * @param dungeonRank Rank of the dungeon (S, A, B, C, D, E)
     * @param questType Type of the quest (KILL or DAMAGE)
     * @param targetCount Target kills or damage required
     * @param goldReward Gold reward for completion
     * @param expReward Experience reward for completion
     * @param itemRewardId ID of the item reward
     */
    Quest(const string& dungeonRank,
          const string& questType,
          int           targetCount,
          int           goldReward,
          int           expReward,
          const string& itemRewardId);

    /**
     * @brief Default constructor for empty quest
     */
    Quest();

    /**
     * @brief Destroy the Quest object
     */
    ~Quest();

    /**
     * @brief Get the quest type
     *
     * @return QuestType The quest type
     */
    QuestType getType() const;

    /**
     * @brief Get the dungeon rank
     *
     * @return std::string The dungeon rank
     */
    string getDungeonRank() const;

    /**
     * @brief Get the target count (kills or damage)
     *
     * @return int The target count
     */
    int getTargetCount() const;

    /**
     * @brief Get the current progress
     *
     * @return int The current progress
     */
    int getCurrentProgress() const;

    /**
     * @brief Set the current progress
     *
     * @param progress The current progress value
     */
    void setCurrentProgress(int progress);

    /**
     * @brief Get the gold reward
     *
     * @return int The gold reward
     */
    int getGoldReward() const;

    /**
     * @brief Get the experience reward
     *
     * @return int The experience reward
     */
    int getExpReward() const;

    /**
     * @brief Get the item reward ID
     *
     * @return std::string The item reward ID
     */
    string getItemRewardId() const;

    /**
     * @brief Check if the quest is completed
     *
     * @return true If the quest is completed
     * @return false If the quest is not completed
     */
    bool getIsCompleted() const;

    /**
     * @brief Set the quest as completed
     *
     * @param completed Whether the quest is completed
     */
    void setCompleted(bool completed);

    /**
     * @brief Update progress for KILL type quests
     *
     * @param mobsKilled Number of mobs killed
     * @return true If the quest has been completed with this update
     * @return false If the quest is not yet completed
     */
    bool updateKillProgress(int mobsKilled);

    /**
     * @brief Update progress for DAMAGE type quests
     *
     * @param damageDealt Amount of damage dealt
     * @return true If the quest has been completed with this update
     * @return false If the quest is not yet completed
     */
    bool updateDamageProgress(int damageDealt);

    /**
     * @brief Reset the quest progress
     */
    void resetProgress();

    /**
     * @brief Check if this is a valid quest (not default constructed)
     *
     * @return true If the quest is valid
     * @return false If the quest is not valid (default quest)
     */
    bool isValid() const;

    /**
     * @brief Get a string representation of the quest
     *
     * @return std::string String representation of the quest
     */
    string toString() const;
};