#pragma once

#include "exception/Exception.hpp"
#include "items/Item.hpp"

class Backpack
{
public:
    /**
     * @brief Constructs a Backpack by reshaping a flat list into a rows×cols grid.
     * @param initial Flat vector of (Item, quantity) pairs.
     *                If initial.size() < rows*cols, remaining slots stay default.
     * @param rows    Number of rows in the grid.
     * @param cols    Number of columns in the grid.
     */
    Backpack(const std::vector<std::pair<Item, int>>& initial, int rows, int cols);

    /**
     * @brief Backpack destructor
     */
    ~Backpack();

    /**
     * @brief Move item from one tile to another (Swap two items)
     * @param fromX source x coordinate
     * @param fromY source y coordinate
     * @param toX destination x coordinate
     * @param toY destination y coordinate
     */
    void moveItem(int fromX, int fromY, int toX, int toY);

    /**
     * @brief Add item to backpack (From Loot and Shop)
     * @param item item to add
     * @param quantity quantity to add
     */
    void addItem(const Item& item, int quantity);

    /**
     * @brief Add item to backpack (From Inventory and Equipment)
     * @param x x coordinate
     * @param y y coordinate
     * @param item item to take
     * @param quantity quantity to take
     */
    void addItemAtTile(int x, int y, const Item& item, int quantity);

    /**
     * @brief Take item from backpack (From Battle)
     * @param quantity quantity to take
     */
    Item takeItem(const Item& item, int quantity);

    /**
     * @brief Take item from backpack (From Inventory and Equipment)
     * @param x x coordinate
     * @param y y coordinate
     * @param quantity quantity to take
     */
    Item takeItemAtTile(int x, int y, int quantity);

    /**
     * @brief Get item at tile
     * @param x x coordinate
     * @param y y coordinate
     * @return item at tile
     */
    Item getItemAtTile(int x, int y) const;

    /**
     * @brief Get quantity at tile
     * @param x x coordinate
     * @param y y coordinate
     */
    int getQuantityAtTile(int x, int y) const;

    /**
     * @brief Filter items by type
     * @param type item type
     * @return vector of items with quantity of specified type
     */
    std::vector<std::pair<Item, int>> filterItemsByType(const std::string& type) const;

    /**
     * @brief Find item tile
     * @param item item to find
     * @return Vector of found tile coordinates
     */
    std::vector<std::pair<int, int>> findItemTile(const Item& item) const;

    /**
     * @brief Find empty tile
     * @return Vector of empty tile coordinates
     */
    std::vector<std::pair<int, int>> findEmptyTile() const;

protected:
    const int                                      backpackRows;
    const int                                      backpackCols;
    const int                                      maxStackSize = 64;
    std::vector<std::vector<std::pair<Item, int>>> backpack;

    /**
     * @brief Validate coordinates if they are within the backpack size
     * @param x x coordinate
     * @param y y coordinate
     */
    void validateCoordinates(int x, int y) const;

    /**
     * @brief Get raw item with quantity at tile
     * @param x x coordinate
     * @param y y coordinate
     * @return item with quantity at tile
     */
    std::pair<Item, int> rawAt(int x, int y) const;
};