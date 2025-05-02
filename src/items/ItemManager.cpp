#include "items/ItemManager.hpp"

ItemManager::ItemManager() {}

ItemManager::~ItemManager() {}

Item& ItemManager::getItem(std::string itemID)
{
    auto it = itemDatabase.find(itemID);
    if (it != itemDatabase.end())
    {
        return it->second;
    }

    throw "Item not found";
}

void ItemManager::setItemDatabase(std::vector<Item> items)
{
    itemDatabase.clear();
    for (const auto& item : items)
    {
        itemDatabase[item.getItemID()] = item;
    }
}

std::vector<Item> ItemManager::getAllItems() const
{
    std::vector<Item> result;
    result.reserve(itemDatabase.size());

    for (const auto& pair : itemDatabase)
    {
        result.push_back(pair.second);
    }

    return result;
}

std::vector<Item> ItemManager::getItemsByType(const std::string& type) const
{
    std::vector<Item> result;

    for (const auto& pair : itemDatabase)
    {
        const Item& item = pair.second;
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

    for (const auto& pair : itemDatabase)
    {
        const Item& item = pair.second;
        if (item.getRarity() == rarity)
        {
            result.push_back(item);
        }
    }

    return result;
}