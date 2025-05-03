#include "parser/MobLootConfigParser.hpp"

#include <cstdlib>
#include <string>
#include <vector>

#include "parser/ConfigParserUtils.hpp"

void MobLootConfigParser::ParseFromFile(const std::string& filename)
{
    // 1) read raw tokens: 3 columns per line
    std::vector<std::vector<std::string>> raw;
    if (!ConfigParserUtils::ReadTokensFile(filename + "/mobloot.txt",
                                           raw,
                                           /*expectedCols=*/3,
                                           m_lastError))
    {
        throw FileNotFoundException(filename, m_lastError);
    }

    // 2) build nested map: mob → (itemID → probability)
    m_data.clear();
    for (auto& rec : raw)
    {
        if (rec.size() < 3)
        {
            throw LineTooShortException(filename, m_lastError);
        }
        const auto& mob     = rec[0];
        const auto& itemID  = rec[1];
        float       prob    = std::stof(rec[2]);
        m_data[mob][itemID] = prob;
    }
}