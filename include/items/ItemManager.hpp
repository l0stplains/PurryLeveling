#pragma once

#include <map>

#include "items/Item.hpp"

class ItemManager
{
public:
    ItemManager();
    ~ItemManager();

    void              setItemDatabase(std::vector<Item> itemDatabase);
    Item&             getItem(std::string itemID);
    std::vector<Item> getAllItems() const;
    std::vector<Item> getItemsByType(const std::string& type) const;
    std::vector<Item> getItemsByRarity(char rarity) const;

private:
    std::map<std::string, Item> itemDatabase;
};