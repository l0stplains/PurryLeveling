#pragma once

#include "items/Item.hpp"

class ItemManager {
    public:
        ItemManager(std::vector<Item> itemDatabase);
        ~ItemManager();

        Item& getItem(std::string itemID);
        std::vector<Item> getAllItems() const;
        std::vector<Item> getItemsByType(const std::string& type) const;
        std::vector<Item> getItemsByRarity(char rarity) const;

    private:
        std::vector<Item> itemDatabase;
};