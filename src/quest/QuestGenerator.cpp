#include "quest/QuestGenerator.hpp"

#include <iostream>

QuestGenerator::QuestGenerator() : rng() {}

QuestGenerator::~QuestGenerator() {}

void QuestGenerator::loadQuestData(
    const std::map<std::string, std::vector<std::tuple<std::string, int, int, int, std::string>>>& quests)
{
    questData.clear();
    questData = quests;
    for (const auto& pair : questData)
    {
        const string& rank          = pair.first;
        const auto&   questsForRank = pair.second;

        std::cout << "Loaded quests for rank " << rank << ":" << std::endl;
        for (const auto& questTuple : questsForRank)
        {
            std::cout
                << "  - Type: " << get<0>(questTuple) << ", Target Count: " << get<1>(questTuple)
                << ", Gold Reward: " << get<2>(questTuple) << ", Exp Reward: " << get<3>(questTuple)
                << ", Item Reward ID: " << get<4>(questTuple) << std::endl;
        }
    }
    std::cout << "Quest data loaded successfully." << std::endl;
    std::cout << "Total number of quest types: " << questData.size() << std::endl;
    std::cout << "Total number of quests: " << questData.size() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

Quest QuestGenerator::generateQuest(const string& dungeonRank)
{
    auto it = questData.find(dungeonRank);
    if (it == questData.end() || it->second.empty())
    {
        return Quest();
    }

    const auto& questsForRank = it->second;

    int randomIndex = rng.generateInRange(questsForRank.size());

    const auto& questTuple   = questsForRank[randomIndex];
    string      questType    = get<0>(questTuple);
    int         targetCount  = get<1>(questTuple);
    int         goldReward   = get<2>(questTuple);
    int         expReward    = get<3>(questTuple);
    string      itemRewardId = get<4>(questTuple);

    return Quest(dungeonRank, questType, targetCount, goldReward, expReward, itemRewardId);
}

bool QuestGenerator::hasQuestsForRank(const string& dungeonRank) const
{
    auto it = questData.find(dungeonRank);
    return (it != questData.end() && !it->second.empty());
}