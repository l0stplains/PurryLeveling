#include <iostream>
#include <string>
#include <filesystem>
#include <SFML/System/Vector2.hpp>

#include "parser/PlayerConfigParser.hpp"
#include "save/PlayerConfigSaver.hpp"
#include "items/ItemManager.hpp"
#include "inventory/Equipment.hpp"
#include "inventory/Backpack.hpp"
#include "units/characters/Fighter.hpp"
#include "core/GameContext.hpp"
#include "units/NavigationGrid.hpp"
#include "parser/ItemConfigParser.hpp"

int main()
{
    const std::string name    = "Ghana";
    const std::string inBase  = "data/configtest";
    const std::string outBase = "data/configout";

    // 1) parse
    ItemConfigParser itemParser;
    itemParser.ParseFromFile("data/configtest/item.txt");
    ItemManager itemMgr(itemParser.GetData());
    PlayerConfigParser parser(itemMgr);
    std::string err;
    if (!parser.ParseFromFile(inBase + "/" + name)) {
        std::cerr << "Parse error: " << parser.GetLastError() << "\n";
        return 1;
    }

    // 2) build equipment
    Equipment eq;
    for (auto &row : parser.GetEquipmentData()) {
        const auto &slot = row[0];
        auto        item = itemMgr.getItem(row[1]);
        // adapt these to your Equipment API:
        if      (slot == "WEAPON")     eq.setWeapon(item);
        else if (slot == "ARMOR_HEAD") eq.setHeadArmor(item);
        else if (slot == "ARMOR_BODY") eq.setBodyArmor(item);
        else if (slot == "ARMOR_FOOT") eq.setFootArmor(item);
        else if (slot == "PENDANT")    eq.setPendant(item);
    }

    // 3) build backpack
    Backpack bp(parser.GetBackpackData());

    // 4) instantiate a Fighter with parsed base stats
    NavigationGrid nav(100, 100, 100, 100);        // stub: create or load your nav‐grid
    GameContext   ctx;         // stub: your game context
    Fighter       character(
        parser.GetUnitStats().at("NAME"),
        sf::Vector2f(0,0),
        nav,
        true,
        ctx
    );
    // apply parsed char‐stat values
    character.AddExp(    std::stoi(parser.GetCharStats().at("EXP")));
    character.AddGold(   std::stoi(parser.GetCharStats().at("GOLD")));
    character.AddMastery(std::stoi(parser.GetCharStats().at("MASTERY")));

    // 5) modify something
    character.AddGold(50);
    // bump the (0,0) slot to 5 items:
    auto item = bp.getItemAtTile(0, 0);
    int  cur  = bp.getQuantityAtTile(0, 0);
    if (cur < 5) {
        bp.addItemAtTile(0, 0, item, 5 - cur);
    } else if (cur > 5) {
        bp.takeItemAtTile(0, 0, cur - 5);
    } else {
        std::cout << "Quantity at tile (0, 0) is already 5.\n";
    } 

    // 6) save out
    if (!PlayerConfigSaver::SaveToFolder(outBase, character, eq, bp, err)) {
        std::cerr << "Save failed: " << err << "\n";
        return 1;
    }

    std::cout << "Round‑trip complete: wrote to " 
              << outBase << "/" << name << "\n";
    return 0;
}