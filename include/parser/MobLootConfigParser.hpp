#pragma once

#include <string>
#include <vector>
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class MobLootConfigParser
 * @brief Parses mob loot tables with fixed 3 columns per row.
 */
class MobLootConfigParser : public ConfigParser {
public:
    bool ParseFromFile(const std::string& filename) override;

    /**
     * @brief Data rows: {MobName, ItemID, Probability}
     */
    const std::vector<std::vector<std::string>>& GetData() const { return m_data; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_data;
};