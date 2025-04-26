#pragma once

#include <string>
#include <vector>
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class InventoryConfigParser
 * @brief Parses equipment/inventory files with exactly 5 rows of 2 columns each.
 */
class InventoryConfigParser : public ConfigParser {
public:
    bool ParseFromFile(const std::string& filename) override;

    /**
     * @brief Data rows: {SlotName, ItemID} (exactly 5 rows)
     */
    const std::vector<std::vector<std::string>>& GetData() const { return m_data; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_data;
};