#include "parser/ItemConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"
#include "effects/Effect.hpp"    

#include <sstream>

bool ItemConfigParser::ParseFromFile(const std::string& filename)
{
    // 1) read raw tokens, no fixed column count
    std::vector<std::vector<std::string>> raw;
    if (!ConfigParserUtils::ReadTokensFile(filename, raw, m_lastError))
        return false;

    m_data.clear();
    m_data.reserve(raw.size());

    for (auto& rec : raw)
    {
        if (rec.size() < 8)
        {
            m_lastError = "Line has too few columns: expected >=8, got " +
                          std::to_string(rec.size());
            return false;
        }

        const std::string& id     = rec[0];
        const std::string& name   = rec[1];
        const std::string& type   = rec[2];
        char               rarity = rec[3].empty() ? '\0' : rec[3][0];
        // rec[4] is BaseAtk—ignore if Item ctor doesn't use it

        // 2) up to 3 effects at rec[5..7], stop on "-"        
        std::vector<std::shared_ptr<Effect>> effects;
        size_t idx = 5;
        for (; idx < rec.size() && idx < 8; ++idx) {
            if (rec[idx] == "-") {
                ++idx;  // skip the "-" and begin description
                break;
            }
            // create the Effect pointer and add it
            // auto e = std::make_shared<Effect>(rec[idx]);
            // effects.push_back(std::move(e));
        }

        // 3) remaining tokens are description
        std::string description;
        if (idx < rec.size()) {
            std::ostringstream oss;
            oss << rec[idx++];
            for (; idx < rec.size(); ++idx)
                oss << ' ' << rec[idx];
            description = oss.str();
        }

        // 4) construct the Item
        m_data.emplace_back(
            id,
            name,
            type,
            rarity,
            effects,
            description
        );
    }

    return true;
}