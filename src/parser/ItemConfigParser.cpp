#include "parser/ItemConfigParser.hpp"

bool ItemConfigParser::ParseFromFile(const std::string& filename) {
    // 1) read raw tokens: 8 cols per row
    std::vector<std::vector<std::string>> raw;
    if (!ConfigParserUtils::ReadTokensFile(
            filename,
            raw,
            8,
            m_lastError
        )) {
        return false;
    }

    // 2) convert each row to an Item
    m_data.clear();
    m_data.reserve(raw.size());
    for (auto& rec : raw) {
        const std::string& id     = rec[0];
        const std::string& name   = rec[1];
        const std::string& type   = rec[2];
        char               rarity = rec[3].empty() ? '\0' : rec[3][0];

        std::vector<std::string> effects;
        effects.insert(effects.end(), rec.begin() + 4, rec.end());

        m_data.emplace_back(id, name, type, rarity, effects);
    }

    return true;
}