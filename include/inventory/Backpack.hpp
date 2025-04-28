#pragma once

#include "../include/items/Item.hpp"
#include <utility>  // for std::pair

class Backpack {
public:
    Backpack(std::vector<std::vector<std::pair<Item, int>>> initial);
    ~Backpack();

    void moveItem(int fromX, int fromY, int toX, int toY);
    void addItem(const Item& item, int quantity);
    void addItemAtTile(int x, int y, const Item& item, int quantity);
    Item takeItem(const Item& item, int quantity);
    Item takeItemAtTile(int x, int y, int quantity);
    
    Item getItemAtTile(int x, int y) const;
    int  getQuantityAtTile(int x, int y) const;

protected:
    const int backpackRows = 8;
    const int backpackCols = 4;
    const int maxStackSize = 64;
    std::vector<std::vector<std::pair<Item,int>>> backpack;

    void validateCoordinates(int x, int y) const;
    std::pair<Item,int> rawAt(int x, int y) const;
    
    std::pair<bool, std::pair<int, int>> findItemTile(const Item& item) const;
    std::pair<bool, std::pair<int, int>> findEmptyTile() const;
};