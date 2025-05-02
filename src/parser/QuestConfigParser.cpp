#include "parser/QuestConfigParser.hpp"

#include <cstdlib>
#include <string>
#include <vector>

#include "parser/ConfigParserUtils.hpp"
void QuestConfigParser::ParseFromFile(const std::string& filename)
{
    // 1) read raw tokens: 6 columns per line
    std::vector<std::vector<std::string>> raw;
    if (!ConfigParserUtils::ReadTokensFile(filename,
                                           raw,
                                           /* expectedCols = */ 6,
                                           m_lastError))
    {
        throw FileNotFoundException();
    }

    // 2) build m_questData: rank → list of (type, count, gold, exp, itemID)
    m_questData.clear();
    for (auto const& rec : raw)
    {
        if (rec.size() < 6)
        {
            throw LineTooShortException();
        }
        const std::string& rank       = rec[0];
        const std::string& qtype      = rec[1];
        int                count      = std::stoi(rec[2]);
        int                goldReward = std::stoi(rec[3]);
        int                expReward  = std::stoi(rec[4]);
        const std::string& itemID     = rec[5];

        m_questData[rank].emplace_back(qtype, count, goldReward, expReward, itemID);
    }
}