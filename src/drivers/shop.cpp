#include "shop/Shop.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "core/GameContext.hpp"  // <- include this

#include "inventory/Backpack.hpp"
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
    std::cout << "Loaded " << itemParser.GetData().size() << " items.\n";

    // 2) Load shop configuration
    ShopConfigParser shopParser;
    if (!shopParser.ParseFromFile("data/configtest/shop.txt"))
    {
        std::cerr << "Shop parse error: " << shopParser.GetLastError() << "\n";
        return 1;
    }
    auto shopData = shopParser.GetData();
    std::cout << "Loaded shop configuration (" << shopData.size() << " entries).\n";

    // 3) Construct the Shop
    Shop shop(shopData, itemMgr.getAllItems());
    std::cout << "Shop initialized.\n\n";

    // 4) Create and initialize GameContext
    GameContext ctx;
    ctx.SetItemManager(&itemMgr);
    // if you have other parsers/managers, wire them here:
    // ctx.SetMobLootConfigParser(&mobLootParser);
    // ctx.SetResourceManager(...);
    // ctx.SetUnitManager(...);

    // 5) Create a navigation grid and player
    NavigationGrid nav(100, 100, 10, 10);
    Fighter        player("Shopper",
                   sf::Vector2f(0.f, 0.f),
                   nav,
                   /*isPlayerControlled=*/true,
                   ctx  // now a fully‐wired GameContext
    );
    player.AddGold(1000);
    std::cout << "Player created with 1000 gold.\n\n";

    // 6) (Optional) Load existing save into backpack
    PlayerConfigParser pcParser(itemMgr);
    if (!pcParser.ParseFromFile("data/configtest/Ghana"))
    {
        std::cerr << "Player config parse error: " << pcParser.GetLastError() << "\n";
        return 1;
    }
    Backpack backpack(pcParser.GetBackpackData(), /*rows=*/4, /*cols=*/8);
    std::cout << "Backpack initialized (" << pcParser.GetBackpackData().size()
              << " slots loaded).\n\n";

    // 7) Show initial shop catalogue
    std::cout << "=== Shop Catalogue ===\n";
    shop.getShopCatalogue();
    std::cout << "Player Gold: " << player.GetGold() << "\n\n";

    // 8) Buy an Ice Potion (IPS)
    std::cout << "Attempting to buy 1x IPS...\n";
    try
    {
        shop.buy(player, backpack, "Potion", "IPS");
        std::cout << "Bought IPS.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Buy error: " << e.what() << "\n";
    }
    std::cout << "Gold after buy: " << player.GetGold() << "\n\n";

    // 9) Sell back an IPS
    std::cout << "Attempting to sell 1x IPS...\n";
    try
    {
        auto sold = itemMgr.getItem("IPS");
        shop.sell(player, backpack, sold);
        std::cout << "Sold IPS.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Sell error: " << e.what() << "\n";
    }
    std::cout << "Gold after sell: " << player.GetGold() << "\n\n";

    // 10) Final catalogue
    std::cout << "=== Final Shop Catalogue ===\n";
    shop.getShopCatalogue();
    std::cout << "Player remaining gold: " << player.GetGold() << "\n";

    return 0;
}