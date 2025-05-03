#pragma once

#include <string>
#include <vector>

#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class ShopConfigParser
 * @brief Parses shop configuration files with fixed 4 columns per row.
 */
class ShopConfigParser : public ConfigParser
{
public:
    /**
     * @brief Parse shop configuration from a file
     * @param filename Path to the shop configuration file
     */
    void ParseFromFile(const std::string& filename) override;

    /**
     * @brief Data rows: {ItemName, Rarity, BasePrice, Stock}
     */
    const std::vector<std::vector<std::string>>& GetData() const { return m_data; }

    /**
     * @brief Get the last error message
     * @return Last error message
     */
    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_data;
};