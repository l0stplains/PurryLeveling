#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "effects/Stats.hpp"
#include "items/Item.hpp"
#include "items/ItemManager.hpp"
#include "parser/ConfigParser.hpp"
#include "parser/ConfigParserUtils.hpp"

class PlayerConfigParser : public ConfigParser
{
public:
    /**
     * @brief Constructor for PlayerConfigParser
     * @param itemManager Reference to the ItemManager instance
     */
    PlayerConfigParser(ItemManager& itemManager);

    /**
     * @brief Parse item configuration from a file
     * @param filename Path to the item configuration file
     */
    void ParseFromFile(const std::string& basePath) override;

    /**
     * @brief Get the equipment data
     * @return A vector of vectors containing the equipment data
     */
    const std::vector<std::vector<std::string>>& GetEquipmentData() const
    {
        return m_equipmentData;
    }

    /**
     * @brief Get the backpack data
     * @return A vector of pairs containing the backpack data
     */
    const std::vector<std::pair<Item, int>>& GetBackpackData() const { return m_backpackData; }

    /**
     * @brief Gets the stats for character
     * @return a map consisting of LEVEL, EXP, GOLD, and MASTERY
     */
    const std::map<std::string, std::string>& GetCharStats() const { return m_charstats; }

    /**
     * @brief Gets the stats for unit
     * @return a map consisting of NAME, STRENGTH, INTELLIGENCE, AGILITY,
     * BUFF_MULTIPLIER, CRITICAL_STRIKE_MULTIPLIER, CRITICAL_STRIKE_CHANCE, SKIPTURNCHANCE,
     * LUCK, PHYSICAL_DEFENSE, MAGIC_DEFENSE, DODGE_CHANCE, ACCURACY, STATUS_RESISTANCE,
     * HASTE_MULTIPLIER, RESOURCE_COST_MULTIPLIER
     */
    const std::map<std::string, std::string>& GetUnitStats() const { return m_unitstats; }

    /**
     * @brief Gets the skill tree
     * @return A set of strings consisting of the learned skills
     */
    const std::set<std::string>& GetSkillTree() const { return m_skilltree; }

    /**
     * @brief Gets the last error message
     * @return The last error message that occurred during parsing
     */
    std::string GetLastError() const override { return m_lastError; }

private:
    std::vector<std::vector<std::string>> m_equipmentData;
    std::vector<std::pair<Item, int>>     m_backpackData;
    std::map<std::string, std::string>    m_charstats;
    std::map<std::string, std::string>    m_unitstats;
    std::set<std::string>                 m_skilltree;
    ItemManager&                          m_itemManager;  // Item database for backpack items
};