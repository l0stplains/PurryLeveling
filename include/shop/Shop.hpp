#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "../items/Item.hpp"
#include "../parser/ShopConfigParser.hpp"
#include "../units/characters/Character.hpp"
#include "../inventory/Backpack.hpp"
#include "../exception/Exception.hpp"

class Shop
{
public:
    using StockEntry = std::tuple<Item, int /*price*/, int /*qty*/>;

    // Constructor
    // Takes a vector of vectors (rows) and a vector of Items (master list)
    // The first column is the itemID, the second is the rarity, the third is the base price
    Shop(const std::vector<std::vector<std::string>> shopData,
         const std::vector<Item>&                     masterItems);

    // Restock items
    void restock();
    // Buy and sell items
    void buy(Character& player, Backpack& backpack, const std::string& category, const std::string& itemID);
    void sell(Character& player, Backpack& backpack, const Item& item);

    // For debugging purpose, gets shop catalogue
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