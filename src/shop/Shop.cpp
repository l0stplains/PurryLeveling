#include "shop/Shop.hpp"

Shop::Shop() : m_configData({}), m_masterItems({}), allStock({}) {}

Shop::Shop(const std::vector<std::vector<std::string>> shopData, const std::vector<Item>& masterItems)
    : m_configData(shopData), m_masterItems(masterItems)
{
    restock();
}

void Shop::SetShopData(const std::vector<std::vector<std::string>>& shopData)
{
    m_configData = shopData;
}

void Shop::SetMasterItems(const std::vector<Item>& masterItems)
{
    m_masterItems = masterItems;
}

void Shop::restock()
{
    allStock.clear();

    // Checks each row of the config data and populates the stock maps
    for (auto const& row : m_configData)
    {
        const std::string& id    = row[0];
        int                price = std::stoi(row[2]);
        int                qty   = std::stoi(row[3]);
        if (qty <= 0)
            continue;

        // Find the item in the master list with assumption that itemID is unique
        auto it = std::find_if(m_masterItems.begin(), m_masterItems.end(), [&](auto const& itm) {
            return itm.getItemID() == id;
        });

        // Create a stock entry
        // Note: std::get<0>(entry) is the item, std::get<1>(entry) is price,
        //       std::get<2>(entry) is quantity
        StockEntry  entry {*it, price, qty};
        const auto& type = it->getType();

        // Add the entry to the allStock map
        allStock.emplace_back(entry);
    }
}

void Shop::buy(Character& player, Backpack& backpack, Item& item, int totalPrice, int quantity)
{
    // Check the funds
    if (player.GetGold() < totalPrice)
        throw InsufficientGoldException();

    // Deduct balance from player immediately
    player.AddGold(-totalPrice);

    // Try to give the item, but refund on failure
    try
    {
        backpack.addItem(item, quantity);
    }
    catch (const std::exception&)
    {
        player.AddGold(totalPrice);
        throw;
    }

    // Find the stock entry matching this item
    auto it = std::find_if(allStock.begin(), allStock.end(), [&](const StockEntry& entry) {
        return std::get<0>(entry) == item;
    });

    if (it != allStock.end())
    {
        // Get current stock quantity
        int& currentQty = std::get<2>(*it);

        // Reduce the quantity
        currentQty -= quantity;

        // Remove entry if stock is depleted
        if (currentQty <= 0)
        {
            allStock.erase(it);
        }
    }
}

void Shop::sell(Character& player, Backpack& backpack, const Item& item, int totalPrice, int quantity)
{
    // Add balance to player immediately
    player.AddGold(totalPrice);

    // Try to take the item from the backpack, but refund on failure
    try
    {
        backpack.takeItem(item, quantity);  // Use the passed quantity parameter
    }
    catch (const std::exception&)
    {
        player.AddGold(-totalPrice);  // Refund the gold
        throw;                        // Propagate the exception
    }

    // Check if the item already exists in stock
    auto it = std::find_if(allStock.begin(), allStock.end(), [&](const StockEntry& entry) {
        return std::get<0>(entry) == item;
    });

    if (it != allStock.end())
    {
        // Item already exists in stock, update the quantity
        int& currentQty = std::get<2>(*it);
        currentQty += quantity;
    }
    else
    {
        // Item doesn't exist in stock, add a new entry
        allStock.emplace_back(std::make_tuple(item, totalPrice, quantity));
    }
}

// For debugging purposes
void Shop::getShopCatalogue() const
{
    auto printCat = [&](const char* title, const std::map<std::string, StockEntry>& mp) {
        std::cout << "-- " << title << " --\n";
        for (auto const& [id, entry] : mp)
        {
            const auto& item  = std::get<0>(entry);
            int         price = std::get<1>(entry);
            int         qty   = std::get<2>(entry);
            std::cout << id << ": " << item.getName() << " (Price: " << price << ", Stock: " << qty
                      << ")\n";
        }
    };
}

std::vector<Shop::StockEntry> Shop::filterStock(const std::string& category)
{
    std::vector<StockEntry> filteredStock;

    // Iterate through all stock entries
    for (const auto& entry : allStock)
    {
        // If category is "All" or the item type matches the requested category
        if (category == "All" || std::get<0>(entry).getType() == category)
        {
            filteredStock.emplace_back(entry);
        }
    }

    return filteredStock;
}