#pragma once

#include <string>
#include <vector>
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class BackpackConfigParser
 * @brief Parses backpack layout files with fixed 4 columns per row.
 */
class BackpackConfigParser : public ConfigParser {
public:
    bool ParseFromFile(const std::string& filename) override;

    /**
     * @brief Data rows: {RowIndex, ColIndex, ItemID, TotalStack}
     */
    const std::vector<std::vector<std::string>>& GetData() const { return m_data; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_data;
};