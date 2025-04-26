#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "ShopConfigParser.hpp"
#include "Item.hpp"
#include "Player.hpp"

/**
 * @class Shop
 * @brief Manages shop inventory, buying/selling using a parsed shop configuration.
 */
class Shop {
public:
    /**
     * @brief Construct a Shop by loading its configuration file and master item list.
     * @param configFile    Path to shop configuration (shop.txt)
     * @param masterItems   All possible items (from ItemConfigParser)
     * @throws std::runtime_error if config cannot be loaded
     */
    Shop(const std::string& configFile, const std::vector<Item>& masterItems);

    /** @brief Restock the shop to original quantities from config, skipping zero-stock. */
    void restock();

    /**
     * @brief Attempt to buy the item at index from the given category.
     * @return true if purchase successful.
     */
    bool buy(Player& player, const std::string& category, int index);

    /**
     * @brief Sell an item to the shop (shop buys at half the config price).
     * @return true if sale accepted.
     */
    bool sell(Player& player, const Item& item);

    /** @brief Print the current shop catalogue to stdout (for debugging). */
    void getShopCatalogue() const;

private:
    using StockEntry = std::tuple<Item, int /*price*/, int /*quantity*/>;

    ShopConfigParser m_parser;
    std::vector<std::vector<std::string>> m_configData;
    std::vector<Item> m_masterItems;

    std::vector<StockEntry> potionStock;
    std::vector<StockEntry> weaponStock;
    std::vector<StockEntry> armorStock;
    std::vector<StockEntry> pendantStock;

    // Helper to pick the right stock vector
    std::vector<StockEntry>* getStock(const std::string& category);
    const std::vector<StockEntry>* getStock(const std::string& category) const;
};
