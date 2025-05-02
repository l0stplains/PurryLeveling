#include "parser/QuestConfigParser.hpp"

#include <iostream>

int main()
{
    const std::string filename = "data/configtest/quest.txt";
    QuestConfigParser parser;
    if (!parser.ParseFromFile(filename))
    {
        std::cerr << "Failed to parse " << filename << ": " << parser.GetLastError() << "\n";
        return 1;
    }

    // Retrieve and print
    const auto& questsByRank = parser.GetQuestData();
    for (auto const& [rank, quests] : questsByRank)
    {
        std::cout << "Dungeon Rank: " << rank << "\n";
        for (auto const& q : quests)
        {
            // each tuple: (type, count, gold, exp, itemID)
            const auto& type       = std::get<0>(q);
            int         count      = std::get<1>(q);
            int         goldReward = std::get<2>(q);
            int         expReward  = std::get<3>(q);
            const auto& itemID     = std::get<4>(q);

            std::cout << "  QuestType=" << type << "  Count=" << count << "  Gold=" << goldReward
                      << "  EXP=" << expReward << "  Item=" << itemID << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}