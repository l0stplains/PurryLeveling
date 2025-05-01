#include "shop/Shop.hpp"

#include <iostream>

#include "core/GameContext.hpp"

#include "items/ItemManager.hpp"
#include "parser/ItemConfigParser.hpp"
#include "parser/PlayerConfigParser.hpp"
#include "parser/ShopConfigParser.hpp"
#include "units/NavigationGrid.hpp"
#include "units/characters/Fighter.hpp"

int main()
{
    // 1) Load global item list
    ItemConfigParser itemParser;
    if (!itemParser.ParseFromFile("data/configtest/item.txt"))
    {
        std::cerr << "Item parse error: " << itemParser.GetLastError() << "\n";
        return 1;
    }
    ItemManager itemMgr;
    itemMgr.setItemDatabase(itemParser.GetData());

    // Assume ItemManager provides all items in a vector:
    std::vector<Item> masterItems = itemMgr.getAllItems();

    // 2) Parse shop configuration
    ShopConfigParser shopParser;
    if (!shopParser.ParseFromFile("data/configtest/shop.txt"))
    {
        std::cerr << "Shop parse error: " << shopParser.GetLastError() << "\n";
        return 1;
    }
    auto shopData = shopParser.GetData();

    // 3) Construct shop (auto‐calls restock())
    Shop shop(shopData, masterItems);

    // 4) Create a test player with some starting gold
    NavigationGrid nav(100, 100, 10, 10);
    GameContext    ctx;
    Fighter        player("Shopper", sf::Vector2f(0.f, 0.f), nav, true, ctx);
    player.AddGold(500);

    PlayerConfigParser parser(itemMgr);
    std::string        err;
    if (!parser.ParseFromFile("data/configtest/Ghana"))
    {
        std::cerr << "Parse error: " << parser.GetLastError() << "\n";
        return 1;
    }

    Backpack backpack(parser.GetBackpackData(), 4, 8);

    // 5) Show initial catalog and player gold
    std::cout << "=== Initial Shop Catalogue ===\n";
    shop.getShopCatalogue();
    std::cout << "Player Gold: " << player.GetGold() << "\n\n";

    // 6) Attempt to buy one Health Potion (ID "HPP")
    std::cout << "Buying 1x Health Potion (HPP)...\n";
    try
    {
        shop.buy(player, backpack, "Potion", "HPP");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Buy failed: " << e.what() << "\n";
    }
    shop.getShopCatalogue();
    std::cout << "Player Gold: " << player.GetGold() << "\n\n";

    // 7) Sell back a potion
    std::cout << "Selling 1x Health Potion (HPP)...\n";
    try
    {
        shop.sell(player, backpack, itemMgr.getItem("HPP"));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Sell failed: " << e.what() << "\n";
    }
    shop.getShopCatalogue();
    std::cout << "Player Gold: " << player.GetGold() << "\n\n";

    return 0;
}