#include "shop/Shop.hpp"

Shop::Shop(const std::vector<std::vector<std::string>> shopData,
           const std::vector<Item>&                     masterItems)
  : m_configData(shopData), m_masterItems(masterItems)
{
    restock();
}

void Shop::restock()
{
    potionStock.clear();
    weaponStock.clear();
    armorStock.clear();
    pendantStock.clear();

    for (auto const& row : m_configData)
    {
        const std::string& id    = row[0];
        int                price = std::stoi(row[2]);
        int                qty   = std::stoi(row[3]);
        if (qty <= 0) continue;

        auto it = std::find_if(
            m_masterItems.begin(), m_masterItems.end(),
            [&](auto const& itm) { return itm.getItemID() == id; });
        if (it == m_masterItems.end())
        {
            std::cerr << "Shop::restock: unknown itemID '" << id << "'\n";
            continue;
        }

        StockEntry entry{*it, price, qty};
        const auto& type = it->getType();

        if (type == "Potion")
            potionStock[id] = entry;
        else if (type == "Weapon")
            weaponStock[id] = entry;
        else if (type == "HeadArmor")
            armorStock[id] = entry;
        else if (type == "BodyArmor")
            armorStock[id] = entry;
        else if (type == "FootArmor")
            armorStock[id] = entry;
        else if (type == "Pendant")
            pendantStock[id] = entry;
        else
            std::cerr << "Shop::restock: unhandled type '" << type << "'\n";
    }
}

void Shop::buy(Character& player,
    Backpack& backpack,
    const std::string& category,
    const std::string& itemID)
{
    // 1) find the correct stock map
    auto stock = getStock(category);
    if (!stock)
        throw std::invalid_argument("Shop::buy: unknown category '" + category + "'");

    // 2) find the item entry
    auto it = stock->find(itemID);
    if (it == stock->end())
        throw std::out_of_range("Shop::buy: '" + itemID + "' not in stock");

    // 3) unpack the tuple
    auto& [itm, price, qty] = it->second;

    // 4) check funds
    if (player.GetGold() < price)
        throw std::runtime_error("Shop::buy: insufficient gold");

    // 5) deduct immediately
    player.AddGold(-price);

    // 6) try to give the item, refund on failure
    try {
        backpack.addItem(itm, 1);
    }
    catch (const std::exception&) {
        player.AddGold(price);
        throw;  // rethrow original
    }

    // 7) decrement stock, erase if empty
    if (--qty <= 0)
    stock->erase(it);
}

void Shop::sell(Character& player, Backpack& backpack, const Item& item)
{
    // lookup config price
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

   // 1) remove the item from player; refund on failure
    try {
        backpack.takeItem(item, 1);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Shop::sell: failed to remove item: " + std::string(e.what()));
    }

    // 2) give gold
    player.AddGold(sellPrice);

    // 3) restock one unit at full config price
    auto stock = getStock(item.getType());
    if (stock)
    {
        auto& entry = (*stock)[item.getItemID()];
        if (entry == StockEntry{})
            entry = StockEntry{item, configPrice, 1};
        else
            std::get<2>(entry) += 1;
    }
}


// For debugging purposes
void Shop::getShopCatalogue() const
{
    auto printCat = [&](const char* title,
                        const std::map<std::string, StockEntry>& mp) {
        std::cout << "-- " << title << " --\n";
        for (auto const& [id, entry] : mp)
        {
            const auto& item  = std::get<0>(entry);
            int         price = std::get<1>(entry);
            int         qty   = std::get<2>(entry);
            std::cout << id << ": "
                      << item.getName()
                      << " (Price: " << price
                      << ", Stock: " << qty << ")\n";
        }
    };
    printCat("Potions", potionStock);
    printCat("Weapons", weaponStock);
    printCat("Armor",   armorStock);
    printCat("Pendants",pendantStock);
}

std::map<std::string, Shop::StockEntry>*
Shop::getStock(const std::string& category)
{
    if (category == "Potion")  return &potionStock;
    if (category == "Weapon")  return &weaponStock;
    if (category == "Armor")   return &armorStock;
    if (category == "Pendant") return &pendantStock;
    return nullptr;
}

const std::map<std::string, Shop::StockEntry>*
Shop::getStock(const std::string& category) const
{
    return const_cast<Shop*>(this)->getStock(category);
}