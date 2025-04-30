#pragma once

#include <algorithm>
#include <iostream>
#include <map>               // ← changed
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "../items/Item.hpp"
#include "../parser/ShopConfigParser.hpp"
#include "../units/characters/Character.hpp"
#include "../inventory/Backpack.hpp"

class Shop
{
public:
    using StockEntry = std::tuple<Item, int /*price*/, int /*qty*/>;

    Shop(const std::vector<std::vector<std::string>> shopData,
         const std::vector<Item>&                     masterItems);

    void restock();
    void buy(Character& player, Backpack& backpack, const std::string& category, const std::string& itemID);
    void sell(Character& player, Backpack& backpack, const Item& item);
    void getShopCatalogue() const;

private:
    ShopConfigParser                      m_parser;
    std::vector<std::vector<std::string>> m_configData;
    std::vector<Item>                     m_masterItems;

    std::map<std::string, StockEntry> potionStock;
    std::map<std::string, StockEntry> weaponStock;
    std::map<std::string, StockEntry> armorStock;
    std::map<std::string, StockEntry> pendantStock;

    std::map<std::string, StockEntry>*       getStock(const std::string& category);
    const std::map<std::string, StockEntry>* getStock(const std::string& category) const;
};