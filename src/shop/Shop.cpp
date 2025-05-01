#include "shop/Shop.hpp"

Shop::Shop(const std::vector<std::vector<std::string>> shopData, const std::vector<Item>& masterItems)
    : m_configData(shopData), m_masterItems(masterItems)
{
    restock();
}

void Shop::restock()
{
    allStock.clear();
    potionStock.clear();
    weaponStock.clear();
    headStock.clear();
    bodyStock.clear();
    footStock.clear();
    pendantStock.clear();

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
        if (it == m_masterItems.end())
        {
            std::cerr << "Shop::restock: unknown itemID '" << id << "'\n";
            continue;
        }

        // Create a stock entry
        // Note: std::get<0>(entry) is the item, std::get<1>(entry) is price,
        //       std::get<2>(entry) is quantity
        StockEntry  entry {*it, price, qty};
        const auto& type = it->getType();

        // Add the entry to the allStock map
        allStock[id] = entry;

        // Add the entry to the appropriate stock map
        if (type == "Potion")
            potionStock[id] = entry;
        else if (type == "Weapon")
            weaponStock[id] = entry;
        else if (type == "HeadArmor")
            headStock[id] = entry;
        else if (type == "BodyArmor")
            bodyStock[id] = entry;
        else if (type == "FootArmor")
            footStock[id] = entry;
        else if (type == "Pendant")
            pendantStock[id] = entry;
        else
            std::cerr << "Shop::restock: unhandled type '" << type << "'\n";
    }
}

void Shop::buy(Character&         player,
               Backpack&          backpack,
               const std::string& category,
               const std::string& itemID)
{
    auto stock = getStock(category);

    // Finds item entry
    auto it = stock->find(itemID);
    if (it == stock->end())
        throw std::out_of_range("Shop::buy: '" + itemID + "' not in stock");

    // Unpacks the tuple
    auto& [itm, price, qty] = it->second;

    // Check the funds
    if (player.GetGold() < price)
        throw InsufficientGoldException();

    // Deduct balance from player immediately
    player.AddGold(-price);

    // Try to give the item, but refund on failure
    try
    {
        backpack.addItem(itm, 1);
    }
    catch (const std::exception&)
    {
        player.AddGold(price);
        throw;
    }

    // Decrement stock, erase if empty
    if (--qty <= 0)
        stock->erase(it);
}

void Shop::sell(Character& player, Backpack& backpack, const Item& item)
{
    // Lookup config price
    int configPrice = -1;
    for (auto const& row : m_configData)
    {
        if (row[0] == item.getItemID())
        {
            configPrice = std::stoi(row[2]);
            break;
        }
    }
    if (configPrice < 0)
    {
        throw std::invalid_argument("Shop::sell: unknown itemID '" + item.getItemID() + "'");
    }

    int sellPrice = configPrice / 2;

    // Remove the item from player; refund on failure
    try
    {
        backpack.takeItem(item, 1);
    }
    catch (const std::exception&)
    {
        throw;
    }

    // Give gold
    player.AddGold(sellPrice);

    // Restock one unit at full config price
    auto stock = getStock(item.getType());
    if (stock)
    {
        auto& entry = (*stock)[item.getItemID()];
        if (entry == StockEntry {})
            entry = StockEntry {item, configPrice, 1};
        else
            std::get<2>(entry) += 1;
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
    printCat("Potions", potionStock);
    printCat("Weapons", weaponStock);
    printCat("Head Armor", headStock);
    printCat("Body Armor", bodyStock);
    printCat("Foot Armor", footStock);
    printCat("Pendants", pendantStock);
}

std::map<std::string, Shop::StockEntry>* Shop::getStock(const std::string& category)
{
    if (category == "Potion")
        return &potionStock;
    else if (category == "Weapon")
        return &weaponStock;
    else if (category == "HeadArmor")
        return &headStock;
    else if (category == "BodyArmor")
        return &bodyStock;
    else if (category == "FootArmor")
        return &footStock;
    else if (category == "Pendant")
        return &pendantStock;
    else
        std::cerr << "Shop::getStock: unhandled category '" << category << "'\n";

    return nullptr;
}

const std::map<std::string, Shop::StockEntry>* Shop::getStock(const std::string& category) const
{
    return const_cast<Shop*>(this)->getStock(category);
}