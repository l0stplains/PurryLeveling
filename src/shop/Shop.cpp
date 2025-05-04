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

    // Populate m_idPrice map when shop data is set
    m_idPrice.clear();

    for (const auto& row : shopData)
    {
        if (row.size() >= 4)  // Ensure we have enough elements
        {
            const std::string& id    = row[0];
            int                price = std::stoi(row[2]);

            // Store the original price for each item ID
            m_idPrice[id] = price;
        }
    }
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
        // Search first for the original price
        auto priceIt  = m_idPrice.find(item.getItemID());
        int  oriPrice = 0;
        if (priceIt != m_idPrice.end())
        {
            oriPrice = priceIt->second;
        }
        else
        {
            if (item.getRarity() == 'S')
                oriPrice = 1000;
            else if (item.getRarity() == 'A')
                oriPrice = 500;
            else if (item.getRarity() == 'B')
                oriPrice = 250;
            else if (item.getRarity() == 'C')
                oriPrice = 125;
            else if (item.getRarity() == 'D')
                oriPrice = 50;
            else if (item.getRarity() == 'E')
                oriPrice = 25;
        }
        allStock.emplace_back(std::make_tuple(item, oriPrice, quantity));
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

// Update these methods in Shop.cpp

std::vector<Shop::StockEntry> Shop::filterStockByCategories(const std::vector<std::string>& categories)
{
    std::vector<StockEntry> filteredStock;

    // If no categories are selected, show all items
    if (categories.empty())
    {
        return allStock;
    }

    // Iterate through all stock entries
    for (const auto& entry : allStock)
    {
        const std::string& itemType = std::get<0>(entry).getType();

        // Check if the item type matches any of the selected categories
        for (const std::string& category : categories)
        {
            if (itemType == category)
            {
                filteredStock.emplace_back(entry);
                break;  // Item found, no need to check other categories
            }
        }
    }

    return filteredStock;
}

std::vector<Shop::StockEntry> Shop::getSortedStock(const std::vector<std::string>& categories,
                                                   const std::string&              sortBy,
                                                   bool                            isDescending)
{
    std::vector<StockEntry> filteredStock = filterStockByCategories(categories);

    if (sortBy == "Rarity")
    {
        std::sort(filteredStock.begin(),
                  filteredStock.end(),
                  [isDescending](const StockEntry& a, const StockEntry& b) {
                      // Rarity order: E < D < C < B < A < S for ascending
                      // For descending, we reverse this: S < A < B < C < D < E
                      char ra = std::get<0>(a).getRarity();
                      char rb = std::get<0>(b).getRarity();

                      // For ascending sort
                      if (!isDescending)
                      {
                          if (ra == 'E' && rb != 'E')
                              return true;
                          if (ra != 'E' && rb == 'E')
                              return false;
                          if (ra == 'D' && rb != 'D' && rb != 'E')
                              return true;
                          if (ra != 'D' && ra != 'E' && rb == 'D')
                              return false;
                          if (ra == 'C' && rb != 'C' && rb != 'D' && rb != 'E')
                              return true;
                          if (ra != 'C' && ra != 'D' && ra != 'E' && rb == 'C')
                              return false;
                          if (ra == 'B' && rb != 'B' && rb != 'C' && rb != 'D' && rb != 'E')
                              return true;
                          if (ra != 'B' && ra != 'C' && ra != 'D' && ra != 'E' && rb == 'B')
                              return false;
                          if (ra == 'A' && rb == 'S')
                              return true;
                          if (ra == 'S' && rb == 'A')
                              return false;
                      }
                      // For descending sort
                      else
                      {
                          if (ra == 'S' && rb != 'S')
                              return true;
                          if (ra != 'S' && rb == 'S')
                              return false;
                          if (ra == 'A' && rb != 'A' && rb != 'S')
                              return true;
                          if (ra != 'A' && ra != 'S' && rb == 'A')
                              return false;
                          if (ra == 'B' && rb != 'B' && rb != 'A' && rb != 'S')
                              return true;
                          if (ra != 'B' && ra != 'A' && ra != 'S' && rb == 'B')
                              return false;
                          if (ra == 'C' && rb != 'C' && rb != 'B' && rb != 'A' && rb != 'S')
                              return true;
                          if (ra != 'C' && ra != 'B' && ra != 'A' && ra != 'S' && rb == 'C')
                              return false;
                          if (ra == 'D' && rb == 'E')
                              return true;
                          if (ra == 'E' && rb == 'D')
                              return false;
                      }

                      return false;
                  });
    }
    else if (sortBy == "Price")
    {
        std::sort(filteredStock.begin(),
                  filteredStock.end(),
                  [isDescending](const StockEntry& a, const StockEntry& b) {
                      if (!isDescending)
                          return std::get<1>(a) < std::get<1>(b);  // Ascending (lowest to highest)
                      else
                          return std::get<1>(a) > std::get<1>(b);  // Descending (highest to lowest)
                  });
    }
    else if (sortBy == "Stock")
    {
        std::sort(filteredStock.begin(),
                  filteredStock.end(),
                  [isDescending](const StockEntry& a, const StockEntry& b) {
                      if (!isDescending)
                          return std::get<2>(a) < std::get<2>(b);  // Ascending (lowest to highest)
                      else
                          return std::get<2>(a) > std::get<2>(b);  // Descending (highest to lowest)
                  });
    }

    return filteredStock;
}

// Get backpack items for selling (with filtering)
std::vector<Shop::StockEntry> Shop::getBackpackStockForSelling(
    Backpack& backpack, const std::vector<std::string>& categories)
{
    std::vector<StockEntry> backpackStock;

    // Get backpack data
    const auto& backpackData = backpack.getBackpackData();

    // Map to store item quantities by itemID
    std::map<std::string, std::pair<Item, int>> itemSummary;

    // Iterate through all backpack slots
    for (int i = 0; i < backpack.getBackpackRows(); ++i)
    {
        for (int j = 0; j < backpack.getBackpackCols(); ++j)
        {
            const auto& slot     = backpackData[i][j];
            const Item& item     = slot.first;
            int         quantity = slot.second;

            // Skip empty slots
            if (item.isNull() || quantity == 0)
                continue;

            // Check if the item matches any selected category
            bool shouldInclude = false;

            if (categories.empty())
            {
                shouldInclude = true;  // No filter - include all items
            }
            else
            {
                for (const std::string& category : categories)
                {
                    if (item.getType() == category)
                    {
                        shouldInclude = true;
                        break;
                    }
                }
            }

            if (shouldInclude)
            {
                // Sum up the quantities if we've seen this item before
                auto it = itemSummary.find(item.getItemID());
                if (it != itemSummary.end())
                {
                    it->second.second += quantity;
                }
                else
                {
                    itemSummary[item.getItemID()] = std::make_pair(item, quantity);
                }
            }
        }
    }

    // Convert the summary to StockEntry vector
    for (const auto& [itemID, itemData] : itemSummary)
    {
        const Item& item          = itemData.first;
        int         totalQuantity = itemData.second;

        // Calculate sell price using m_idPrice map
        int sellPrice = 0;

        // Check if item exists in m_idPrice map
        auto priceIt = m_idPrice.find(item.getItemID());
        if (priceIt != m_idPrice.end())
        {
            sellPrice = priceIt->second * 70 / 100;  // 70% of the buy price
        }
        else
        {
            // Fallback to checking current stock if item is not in m_idPrice
            auto shopIt = std::find_if(allStock.begin(), allStock.end(), [&](const StockEntry& entry) {
                return std::get<0>(entry).getItemID() == item.getItemID();
            });

            if (shopIt != allStock.end())
            {
                sellPrice = std::get<1>(*shopIt) * 70 / 100;  // 70% of the buy price
            }
            else
            {
                // If item is not found in either map, set a default sell price
                sellPrice = 1;  // Default value
            }
        }

        // Create stock entry
        StockEntry entry = std::make_tuple(item, sellPrice, totalQuantity);
        backpackStock.push_back(entry);
    }

    return backpackStock;
}