#pragma once

#include <string>
#include <vector>

#include "items/Item.hpp"
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class ItemConfigParser
 * @brief Parses item configuration files with fixed 8 columns per row.
 */
class ItemConfigParser : public ConfigParser
{
public:
    /**
     * @brief Parse item configuration from a file
     *
     * @param filename Path to the item configuration file
     * @return true if parsing was successful, false otherwise
     */
    void ParseFromFile(const std::string& filename) override;

    /**
     * @brief Retrieved data as rows of tokens: {ID, Name, Type, Rarity, BaseAtk, Effect1, Effect2,
     * Effect3}
     */
    const std::vector<Item>& GetData() const { return m_data; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<Item> m_data;
};