#pragma once

#include <string>
#include <vector>
#include <map>
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"
#include "items/Item.hpp"
#include "items/ItemManager.hpp"

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

    void setItemManager(ItemManager& itemManager) { m_itemManager = itemManager; }
    /// [ SlotName, ItemID ] × 5
    const std::vector<std::vector<std::string>>& GetEquipmentData() const { return m_equipmentData; }
    /// [ RowIdx, ColIdx, ItemID, TotalStack ]
    const std::vector<std::vector<std::pair<Item, int>>>& GetBackpackData()  const { return m_backpackData; }
    const std::map<std::string,std::string>&    GetStats()       const { return m_stats; }
    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_equipmentData;
    std::vector<std::vector<std::pair<Item, int>>> m_backpackData;
    std::map<std::string, std::string> m_stats;
    ItemManager& m_itemManager; // Item database for backpack items
};