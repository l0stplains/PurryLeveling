#include "parser/PlayerConfigParser.hpp"

#include <algorithm>  // for std::max
#include <cstdlib>    // for std::stoul, std::stoi

PlayerConfigParser::PlayerConfigParser(ItemManager& itemManager) : m_itemManager(itemManager) {}

void PlayerConfigParser::ParseFromFile(const std::string& basePath)
{
    if (!ConfigParserUtils::ReadTokensFile(
            basePath + "/equipment.txt", m_equipmentData, 2, 5, m_lastError))
        throw FileNotFoundException(basePath, m_lastError);

    // 2) backpack: n×4 → raw tokens
    std::vector<std::vector<std::string>> backpackRecs;
    if (!ConfigParserUtils::ReadTokensFile(basePath + "/backpack.txt", backpackRecs, 4, m_lastError))
        throw FileNotFoundException(basePath, m_lastError);

    // convert to grid of <Item, count>
    m_backpackData.clear();
    for (const auto& rec : backpackRecs)
    {
        Item itm = m_itemManager.getItem(rec[2]);
        int  cnt = std::stoi(rec[3]);
        m_backpackData.emplace_back(std::move(itm), cnt);
    }

    // 3) stats: key/value per line
    std::vector<std::vector<std::string>> statsRecs;
    if (!ConfigParserUtils::ReadTokensFile(basePath + "/stats.txt",
                                           statsRecs,
                                           /*expectedCols=*/2,
                                           m_lastError))
        throw FileNotFoundException(basePath, m_lastError);
    if (statsRecs.size() < 20)
    {
        m_lastError =
            "stats.txt has too few rows: expected >=20, got " + std::to_string(statsRecs.size());
        throw LineTooShortException(basePath, m_lastError);
    }

    m_charstats.clear();
    m_unitstats.clear();

    for (size_t i = 0; i < statsRecs.size(); ++i)
    {
        const auto& rec = statsRecs[i];
        const auto& key = rec[0];
        const auto& val = rec[1];

        if (i < 4)
        {
            // LEVEL, EXP, GOLD, MASTERY
            m_charstats[key] = val;
        }
        else if (i < 21)
        {
            // NAME + core stats
            m_unitstats[key] = val;
        }
    }

    // 4) skills.txt : { SkillName, Activated(0|1) } per line
    {
        std::vector<std::vector<std::string>> skillRecs;
        if (!ConfigParserUtils::ReadTokensFile(basePath + "/skills.txt",
                                               skillRecs,
                                               /*expectedCols=*/1,
                                               m_lastError))
        {
            throw FileNotFoundException(basePath, m_lastError);
        }

        m_skilltree.clear();
        for (auto const& rec : skillRecs)
        {
            // rec[0] = skill name
            m_skilltree.insert(rec[0]);
        }
    }
}