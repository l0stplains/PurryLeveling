#include "parser/MobLootConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

#include <vector>
#include <string>
#include <cstdlib> 

bool MobLootConfigParser::ParseFromFile(const std::string& filename)
{
    // 1) read raw tokens: 3 columns per line
    std::vector<std::vector<std::string>> raw;
    if (!ConfigParserUtils::ReadTokensFile(
            filename,
            raw,
            /*expectedCols=*/3,
            m_lastError))
    {
        return false;
    }

    // 2) build nested map: mob → (itemID → probability)
    m_data.clear();
    for (auto& rec : raw)
    {
        const auto& mob    = rec[0];
        const auto& itemID = rec[1];
        float        prob   = std::stof(rec[2]);
        m_data[mob][itemID] = prob;
    }

    return true;
}