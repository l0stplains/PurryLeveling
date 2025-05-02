#include "quest/Quest.hpp"

#include <sstream>

Quest::Quest(const string& dungeonRank,
             const string& questType,
             int           targetCount,
             int           goldReward,
             int           expReward,
             const string& itemRewardId)
    : dungeonRank(dungeonRank),
      targetCount(targetCount),
      currentProgress(0),
      goldReward(goldReward),
      expReward(expReward),
      itemRewardId(itemRewardId),
      isCompleted(false)
{
    if (questType == "KILL")
    {
        type = QuestType::KILL;
    }
    else if (questType == "DAMAGE")
    {
        type = QuestType::DAMAGE;
    }
    else
    {
        type = QuestType::KILL;
    }
}

Quest::Quest()
    : type(QuestType::KILL),
      dungeonRank(""),
      targetCount(0),
      currentProgress(0),
      goldReward(0),
      expReward(0),
      itemRewardId(""),
      isCompleted(false)
{}

Quest::~Quest() {}

QuestType Quest::getType() const
{
    return type;
}

string Quest::getDungeonRank() const
{
    return dungeonRank;
}

int Quest::getTargetCount() const
{
    return targetCount;
}

int Quest::getCurrentProgress() const
{
    return currentProgress;
}

void Quest::setCurrentProgress(int progress)
{
    currentProgress = progress;
    if (currentProgress >= targetCount)
    {
        currentProgress = targetCount;
        isCompleted     = true;
    }
}

int Quest::getGoldReward() const
{
    return goldReward;
}

int Quest::getExpReward() const
{
    return expReward;
}

string Quest::getItemRewardId() const
{
    return itemRewardId;
}

bool Quest::getIsCompleted() const
{
    return isCompleted;
}

void Quest::setCompleted(bool completed)
{
    isCompleted = completed;
}

bool Quest::updateKillProgress(int mobsKilled)
{
    if (type == QuestType::KILL && !isCompleted)
    {
        currentProgress += mobsKilled;

        if (currentProgress >= targetCount)
        {
            currentProgress = targetCount;
            isCompleted     = true;
            return true;
        }
    }
    return false;
}

bool Quest::updateDamageProgress(int damageDealt)
{
    if (type == QuestType::DAMAGE && !isCompleted)
    {
        currentProgress += damageDealt;

        if (currentProgress >= targetCount)
        {
            currentProgress = targetCount;
            isCompleted     = true;
            return true;
        }
    }
    return false;
}

void Quest::resetProgress()
{
    currentProgress = 0;
    isCompleted     = false;
}

bool Quest::isValid() const
{
    return !dungeonRank.empty() && targetCount > 0;
}

string Quest::toString() const
{
    stringstream ss;
    ss << "Quest Type: " << (type == QuestType::KILL ? "Kill Mobs" : "Deal Damage") << '\n'
       << "Dungeon Rank: " << dungeonRank << '\n'
       << "Progress: " << currentProgress << "/" << targetCount << '\n'
       << "Rewards:" << '\n'
       << "  Gold: " << goldReward << '\n'
       << "  Exp: " << expReward << '\n'
       << "  Item: " << itemRewardId << '\n'
       << "Status: " << (isCompleted ? "Completed" : "In Progress");
    return ss.str();
}