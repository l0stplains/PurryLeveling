#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "../exception/Exception.hpp"
#include "../inventory/Backpack.hpp"
#include "../items/Item.hpp"
#include "../parser/ShopConfigParser.hpp"
#include "../units/characters/Character.hpp"

class Shop
{
public:
    using StockEntry = std::tuple<Item, int /*price*/, int /*qty*/>;

    // Default Constructor
    Shop();

    // Constructor with shop data and master items
    Shop(const std::vector<std::vector<std::string>> shopData, const std::vector<Item>& masterItems);

    // Getters
    const std::vector<std::vector<std::string>>& GetShopData() const { return m_configData; }
    const std::vector<Item>&                     GetMasterItems() const { return m_masterItems; }

    // Shop data is a vector of vectors of strings.
    // In the inner vector, the first element is the item ID,
    // the second is the item name, the third is the item type,
    // the fourth is the item price, and the fifth is the item quantity.
    void SetShopData(const std::vector<std::vector<std::string>>& shopData);

    // Master items are a vector of Item objects.
    // Can be taken from item manager or item config parser.
    void SetMasterItems(const std::vector<Item>& masterItems);

    // Restock items
    void restock();
    // Buy and sell items
    void buy(Character& player, Backpack& backpack, Item& item, int totalPrice, int quantity);

    void sell(Character& player, Backpack& backpack, const Item& item, int totalPrice, int quantity);

    // For debugging purpose, gets shop catalogue
    void getShopCatalogue() const;

    // Updated to support multiple categories
    std::vector<StockEntry> filterStock(const std::string& category);
    std::vector<StockEntry> filterStockByCategories(const std::vector<std::string>& categories);

    // Get sorted stock by category and sort criteria
    // Added isDescending parameter for descending sort direction
    std::vector<StockEntry> getSortedStock(const std::vector<std::string>& categories,
                                           const std::string&              sortBy,
                                           bool                            isDescending = false);

    // Get backpack items for selling (with filtering)
    std::vector<StockEntry> getBackpackStockForSelling(Backpack&                       backpack,
                                                       const std::vector<std::string>& categories);

private:
    ShopConfigParser                      m_parser;
    std::vector<std::vector<std::string>> m_configData;
    std::vector<Item>                     m_masterItems;

    std::vector<StockEntry> allStock;

    // Map to track price of all items that have ever existed in the shop
    std::map<std::string, int> m_idPrice;
};