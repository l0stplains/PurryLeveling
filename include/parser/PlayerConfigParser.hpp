#pragma once

#include <map>
#include <string>
#include <vector>
#include <set>

#include "effects/Stats.hpp"
#include "items/Item.hpp"
#include "items/ItemManager.hpp"
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

class PlayerConfigParser : public ConfigParser
{
public:
    // no more default‐ctor: you must supply an ItemManager
    explicit PlayerConfigParser(ItemManager mgr) : m_itemManager(std::move(mgr)) {}
    bool ParseFromFile(const std::string& basePath) override;

    /// [ SlotName, ItemID ] × 5
    const std::vector<std::vector<std::string>>& GetEquipmentData() const
    {
        return m_equipmentData;
    }
    /// [ RowIdx, ColIdx, ItemID, TotalStack ]
    const std::vector<std::pair<Item, int>>& GetBackpackData() const { return m_backpackData; }

    // line 1–4
    const std::map<std::string, std::string>& GetCharStats() const { return m_charstats; }
    // line 5–20
    const std::map<std::string, std::string>& GetUnitStats() const { return m_unitstats; }
    // line 21–end
    const std::map<std::string, std::string>& GetTypeStats() const { return m_typestats; }
    // get set of skills
    const std::set<std::string>&              GetSkillTree() const { return m_skilltree; }

    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_equipmentData;
    std::vector<std::pair<Item, int>>     m_backpackData;
    std::map<std::string, std::string>    m_charstats;
    std::map<std::string, std::string>    m_unitstats;
    std::map<std::string, std::string>    m_typestats;
    std::set<std::string>                 m_skilltree;
    ItemManager                           m_itemManager;  // Item database for backpack items
};