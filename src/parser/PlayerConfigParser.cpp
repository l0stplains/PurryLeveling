#include "parser/PlayerConfigParser.hpp"
#include <algorithm>    // for std::max
#include <cstdlib>      // for std::stoul, std::stoi

bool PlayerConfigParser::ParseFromFile(const std::string& basePath)
{
    // 1) equipment: exactly 5×2
    if (!ConfigParserUtils::ReadTokensFile(
            basePath + "/equipment.txt",
            m_equipmentData,
            /*expectedCols=*/2,
            /*expectedRows=*/5,
            m_lastError
        )) return false;

    // 2) backpack: n×4 → raw tokens
    std::vector<std::vector<std::string>> backpackRecs;
    if (!ConfigParserUtils::ReadTokensFile(
            basePath + "/backpack.txt",
            backpackRecs,
            /*expectedCols=*/4,
            m_lastError
        )) return false;

    // convert to grid of <Item, count>
    m_backpackData.clear();
    size_t maxR=0, maxC=0;
    for (auto& rec : backpackRecs) {
        size_t r = std::stoul(rec[0]), c = std::stoul(rec[1]);
        maxR = std::max(maxR,r); maxC = std::max(maxC,c);
    }
    m_backpackData.assign(
        maxR+1,
        std::vector<std::pair<Item,int>>(maxC+1,{Item(),0})
    );
    for (auto& rec : backpackRecs) {
        size_t r = std::stoul(rec[0]), c = std::stoul(rec[1]);
        // lookup full Item by ID
        Item itm = m_itemManager.getItem(rec[2]);
        int  cnt = std::stoi(rec[3]);
        m_backpackData[r][c] = { std::move(itm), cnt };
    }

    // 3) stats: key/value per line
    std::vector<std::vector<std::string>> statsRecs;
    if (!ConfigParserUtils::ReadTokensFile(
            basePath + "/stats.txt",
            statsRecs,
            /*expectedCols=*/2,
            m_lastError
        )) return false;

    m_stats.clear();
    for (auto &rec : statsRecs) {
        m_stats[rec[0]] = rec[1];
    }

    return true;
}