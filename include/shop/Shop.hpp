#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "../items/Item.hpp"
#include "../parser/ShopConfigParser.hpp"
#include "../units/characters/Character.hpp"

/**
 * @class Shop
 * @brief Manages shop inventory, buying/selling using a parsed shop configuration.
 */
class Shop
{
public:
    using StockEntry = std::tuple<Item, int /*price*/, int /*quantity*/>;

    Shop(const std::string& configFile, const std::vector<Item>& masterItems);

    void restock();

    // buy/sell now take Character& instead of undefined Player
    bool buy(Character& player, const std::string& category, int index);
    bool sell(Character& player, const Item& item);

    void getShopCatalogue() const;

private:
    ShopConfigParser                      m_parser;
    std::vector<std::vector<std::string>> m_configData;
    std::vector<Item>                     m_masterItems;

    std::vector<StockEntry> potionStock;
    std::vector<StockEntry> weaponStock;
    std::vector<StockEntry> armorStock;
    std::vector<StockEntry> pendantStock;

    // non‑const and const overloads
    std::vector<StockEntry>*       getStock(const std::string& category);
    const std::vector<StockEntry>* getStock(const std::string& category) const;
};