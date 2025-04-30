#include "items/ItemManager.hpp"

ItemManager::ItemManager(std::vector<Item> itemDatabase) : itemDatabase(itemDatabase) {}

ItemManager::~ItemManager() {}

Item& ItemManager::getItem(std::string itemID)
{
    for (auto& item : itemDatabase)
    {
        if (item.getItemID() == itemID)
        {
            return item;
        }
    }

    throw "Item not found";
}

std::vector<Item> ItemManager::getAllItems() const
{
    return itemDatabase;
}

std::vector<Item> ItemManager::getItemsByType(const std::string& type) const
{
    std::vector<Item> result;
    for (const auto& item : itemDatabase)
    {
        if (item.getType() == type)
        {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Item> ItemManager::getItemsByRarity(char rarity) const
{
    std::vector<Item> result;
    for (const auto& item : itemDatabase)
    {
        if (item.getRarity() == rarity)
        {
            result.push_back(item);
        }
    }
    return result;
}
