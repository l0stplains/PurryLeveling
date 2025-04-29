#pragma once

#include <string>
#include <vector>
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

/**
 * @class PlayerConfigParser
 * @brief Parses both the equipment (5 rows of 2 cols) and backpack (n rows of 4 cols) configs.
 */
class PlayerConfigParser : public ConfigParser
{
public:
    /**
     * @param basePath  Directory containing “equipment.txt” and “backpack.txt”
     */
    bool ParseFromFile(const std::string& basePath) override;

    /// [ SlotName, ItemID ] × 5
    const std::vector<std::vector<std::string>>& GetInventoryData() const { return m_inventoryData; }
    /// [ RowIdx, ColIdx, ItemID, TotalStack ]
    const std::vector<std::vector<std::string>>& GetBackpackData()  const { return m_backpackData; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_inventoryData;
    std::vector<std::vector<std::string>> m_backpackData;
};