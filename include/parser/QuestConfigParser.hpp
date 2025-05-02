#pragma once
#include <map>
#include <string>
#include <vector>

#include "ConfigParser.hpp"

class QuestConfigParser : ConfigParser
{
public:
    /**
     * @brief Parse quest configuration from a file
     *
     * @param filename Path to the configuration file
     * @return true if parsing was successful, false otherwise
     */
    bool ParseFromFile(const std::string& filename) override;
    /*
     * Example of a quest configuration file:
     * # <Dungeon Rank> <Quest Type> <Damage Dealt/Mobs Killed> <Gold Reward> <Exp Reward> <Item
     * Reward (ID)> S KILL 10 100 50 EXC S DAMAGE 20 200 100 MNP
     */

    /**
     * @brief Get the last error message
     *
     * @return The last error message that occurred during parsing
     */
    std::string GetLastError() const override { return m_lastError; }

    /**
     * @brief Get the last error message
     *
     * @return The last error message that occurred during parsing
     */
    const std::map<std::string, std::vector<std::tuple<std::string, int, int, int, std::string>>>&
    GetQuestData() const
    {
        return m_questData;
    }

protected:
    /**
     * @brief Store the last error that occurred during parsing
     */
    std::string m_lastError;

    /**
     * @brief Quest data structure. The key of the map is the dungeon rank, the value is a vector of
     * tuples that consists of all the quests for that respective dungeon rank.
     */
    std::map<std::string /*dungeon rank*/,
             std::vector<std::tuple<std::string /*quest type*/,
                                    int /*damage dealt/mobs killed*/,
                                    int /*gold reward*/,
                                    int /*exp reward*/,
                                    std::string /*item reward (ID)*/>>>
        m_questData;
};