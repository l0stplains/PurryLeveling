#pragma once

#include <map>
#include <string>
#include <vector>

#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class MobLootConfigParser
 * @brief Parses mob loot tables with fixed 3 columns per row.
 */
class MobLootConfigParser : public ConfigParser
{
public:
    /**
     * @brief Parse mob loot configuration from a file
     * @param filename Path to the mob loot configuration file
     * @return true if parsing was successful, false otherwise
     */
    void ParseFromFile(const std::string& filename) override;

    /**
     * @brief Data rows: {MobName, ItemID, Probability}
     */
    const std::map<std::string, std::map<std::string, float>>& GetData() const { return m_data; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::map<std::string, std::map<std::string, float>> m_data;
};