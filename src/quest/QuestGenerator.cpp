#include "quest/QuestGenerator.hpp"

QuestGenerator::QuestGenerator() : rng() {}

QuestGenerator::~QuestGenerator() {}

bool QuestGenerator::loadQuestData(const QuestConfigParser& questConfigParser)
{
    questData = questConfigParser.GetQuestData();
    return !questData.empty();
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