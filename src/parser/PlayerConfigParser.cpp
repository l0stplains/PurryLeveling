#include "parser/PlayerConfigParser.hpp"
#include <algorithm>    // for std::max
#include <cstdlib>      // for std::stoul, std::stoi

bool PlayerConfigParser::ParseFromFile(const std::string& basePath)
{
    if (!ConfigParserUtils::ReadTokensFile(
            basePath + "/equipment.txt",
            m_equipmentData,
            2,
            5,
            m_lastError
        )) return false;

    // 2) backpack: n×4 → raw tokens
    std::vector<std::vector<std::string>> backpackRecs;
    if (!ConfigParserUtils::ReadTokensFile(
            basePath + "/backpack.txt",
            backpackRecs,
            4,
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

    m_charstats.clear();
    m_unitstats.clear();
    m_typestats.clear();

    for (size_t i = 0; i < statsRecs.size(); ++i) {
        const auto& rec = statsRecs[i];
        const auto& key = rec[0];
        const auto& val = rec[1];

        if (i < 4) {
            // LEVEL, EXP, GOLD, MASTERY
            m_charstats[key] = val;
        }
        else if (i < 20) {
            // NAME + core stats
            m_unitstats[key] = val;
        }
        else {
            // TYPE, ATTACK_RANGE, BLOCK_CHANCE, etc.
            m_typestats[key] = val;
        }
    }

    return true;
}