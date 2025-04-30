#include "shop/Shop.hpp"

Shop::Shop(const std::string& configFile, const std::vector<Item>& masterItems)
    : m_masterItems(masterItems)
{
    if (!m_parser.ParseFromFile(configFile))
        throw std::runtime_error("Shop load failed: " + m_parser.GetLastError());

    m_configData = m_parser.GetData();
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
        // row = { Name, Rarity, BasePrice, Stock }
        int qty = std::stoi(row[3]);
        if (qty <= 0)
            continue;

        const std::string& name  = row[0];
        int                price = std::stoi(row[2]);

        // find in master list by name
        auto it = std::find_if(m_masterItems.begin(), m_masterItems.end(), [&](const Item& itm) {
            return itm.getName() == name;
        });
        if (it == m_masterItems.end())
        {
            std::cerr << "Shop::restock: unknown item '" << name << "'\n";
            continue;
        }

        Item        item = *it;
        StockEntry  entry {item, price, qty};
        const auto& type = item.getType();

        if (type == "Potion")
            potionStock.push_back(entry);
        else if (type == "Weapon")
            weaponStock.push_back(entry);
        else if (type == "Armor")
            armorStock.push_back(entry);
        else if (type == "Pendant")
            pendantStock.push_back(entry);
        else
            std::cerr << "Shop::restock: unhandled type '" << type << "'\n";
    }
}

bool Shop::buy(Character& player, const std::string& category, int index)
{
    auto stock = getStock(category);
    if (!stock || index < 0 || index >= static_cast<int>(stock->size()))
        return false;

    auto& entry = (*stock)[index];
    auto& item  = std::get<0>(entry);
    auto& price = std::get<1>(entry);
    auto& qty   = std::get<2>(entry);

    // ensure enough gold
    if (player.GetGold() < price)
        return false;
    player.AddGold(-price);

    // TODO: add 'item' to player's inventory/backpack

    --qty;
    return true;
}

bool Shop::sell(Character& player, const Item& item)
{
    int configPrice = -1;
    for (auto const& row : m_configData)
    {
        if (row[0] == item.getName())
        {
            configPrice = std::stoi(row[2]);
            break;
        }
    }
    if (configPrice < 0)
    {
        std::cerr << "Shop::sell: unknown item '" << item.getName() << "'\n";
        return false;
    }

    int sellPrice = configPrice / 2;
    // TODO: remove 'item' from player's inventory/backpack
    player.AddGold(sellPrice);

    // restock one at full price
    auto stock = getStock(item.getType());
    if (stock)
        stock->emplace_back(item, configPrice, 1);

    return true;
}

void Shop::getShopCatalogue() const
{
    auto printCat = [&](const char* title, const std::vector<StockEntry>& vec) {
        std::cout << "-- " << title << " --\n";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            const auto& entry = vec[i];
            const auto& item  = std::get<0>(entry);
            int         price = std::get<1>(entry);
            int         qty   = std::get<2>(entry);
            std::cout << i << ": " << item.getName() << " (Price: " << price << ", Stock: " << qty
                      << ")\n";
        }
    };
    printCat("Potions", potionStock);
    printCat("Weapons", weaponStock);
    printCat("Armor", armorStock);
    printCat("Pendants", pendantStock);
}

std::vector<Shop::StockEntry>* Shop::getStock(const std::string& category)
{
    if (category == "Potion")
        return &potionStock;
    if (category == "Weapon")
        return &weaponStock;
    if (category == "Armor")
        return &armorStock;
    if (category == "Pendant")
        return &pendantStock;
    return nullptr;
}

const std::vector<Shop::StockEntry>* Shop::getStock(const std::string& category) const
{
    // call non‑const overload to avoid code duplication
    return const_cast<Shop*>(this)->getStock(category);
}