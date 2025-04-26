#include "Shop.hpp"

Shop::Shop(const std::string& configFile, const std::vector<Item>& masterItems)
  : m_masterItems(masterItems)
{
    if (!m_parser.ParseFromFile(configFile))
        throw std::runtime_error("Shop load failed: " + m_parser.GetLastError());

    m_configData = m_parser.GetData();
    restock();
}

void Shop::restock() {
    potionStock.clear();
    weaponStock.clear();
    armorStock.clear();
    pendantStock.clear();

    for (auto const& row : m_configData) {
        // row = { Name, Rarity, BasePrice, Stock }
        int qty   = std::stoi(row[3]);
        if (qty <= 0) 
            continue;               // skip zero-stock

        const std::string& name  = row[0];
        int price               = std::stoi(row[2]);

        // find in master list
        auto it = std::find_if(
            m_masterItems.begin(), m_masterItems.end(),
            [&](auto const& itm){ return itm.GetName() == name; }
        );
        if (it == m_masterItems.end()) {
            std::cerr << "Shop::restock: unknown item '" << name << "'\n";
            continue;
        }

        Item item = *it;
        StockEntry entry{ item, price, qty };
        const auto& type = item.GetType();

        if      (type == "Potion")  potionStock.push_back(entry);
        else if (type == "Weapon")  weaponStock.push_back(entry);
        else if (type == "Armor")   armorStock.push_back(entry);
        else if (type == "Pendant") pendantStock.push_back(entry);
        else std::cerr << "Shop::restock: unhandled type '" << type << "'\n";
    }
}

bool Shop::buy(Player& player, const std::string& category, int index) {
    auto stock = getStock(category);
    if (!stock || index < 0 || index >= (int)stock->size()) return false;

    auto& [item, price, qty] = (*stock)[index];
    if (qty <= 0)        return false;
    if (!player.SpendGold(price)) return false;

    player.AddItem(item);
    --qty;
    return true;
}

bool Shop::sell(Player& player, const Item& item) {
    // look up config price
    int configPrice = -1;
    for (auto const& row : m_configData) {
        if (row[0] == item.GetName()) {
            configPrice = std::stoi(row[2]);
            break;
        }
    }
    if (configPrice < 0) {
        std::cerr << "Shop::sell: unknown item '" << item.GetName() << "'\n";
        return false;
    }

    int sellPrice = configPrice / 2;
    if (!player.RemoveItem(item)) return false;
    player.AddGold(sellPrice);

    // add one back into stock at full config price
    auto stock = getStock(item.GetType());
    if (stock) stock->emplace_back(item, configPrice, 1);

    return true;
}

void Shop::getShopCatalogue() const {
    auto printCat = [&](const char* title, auto const& vec){
        std::cout << "-- " << title << " --\n";
        for (size_t i = 0; i < vec.size(); ++i) {
            const auto& [item, price, qty] = vec[i];
            std::cout << i << ": "
                      << item.GetName()
                      << " (Price: " << price
                      << ", Stock: " << qty << ")\n";
        }
    };
    printCat("Potions", potionStock);
    printCat("Weapons", weaponStock);
    printCat("Armor",   armorStock);
    printCat("Pendants",pendantStock);
}

std::vector<Shop::StockEntry>* Shop::getStock(const std::string& category) {
    if      (category == "Potion")  return &potionStock;
    else if (category == "Weapon")  return &weaponStock;
    else if (category == "Armor")   return &armorStock;
    else if (category == "Pendant") return &pendantStock;
    else return nullptr;
}

const std::vector<Shop::StockEntry>* Shop::getStock(const std::string& category) const {
    return const_cast<Shop*>(this)->getStock(category);
}
