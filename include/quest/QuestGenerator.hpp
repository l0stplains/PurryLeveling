#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "parser/QuestConfigParser.hpp"
#include "quest/Quest.hpp"
#include "rng/rng.hpp"

using namespace std;

/**
 * @brief Class to generate quests for dungeons based on dungeon rank
 */
class QuestGenerator
{
private:
    std::map<std::string, std::vector<std::tuple<std::string, int, int, int, std::string>>> questData;
    RNG                                                                                     rng;

public:
    /**
     * @brief Construct a new Quest Generator object
     */
    QuestGenerator();

    /**
     * @brief Destroy the Quest Generator object
     */
    ~QuestGenerator();

    /**
     * @brief Load quest data from a QuestConfigParser
     *
     * @param questConfigParser The parser with quest configuration data
     * @return true If data was loaded successfully
     * @return false If there was an error loading data
     */
    bool loadQuestData(const QuestConfigParser& questConfigParser);

    /**
     * @brief Generate a quest for a specific dungeon rank
     *
     * @param dungeonRank The rank of the dungeon (S, A, B, C, D, E)
     * @return Quest A quest for the specified dungeon rank
     */
    Quest generateQuest(const string& dungeonRank);

    /**
     * @brief Check if there are quests available for a specific dungeon rank
     *
     * @param dungeonRank The dungeon rank to check
     * @return true If quests are available for this rank
     * @return false If no quests are available for this rank
     */
    bool hasQuestsForRank(const string& dungeonRank) const;
};