#include "parser/PlayerConfigParser.hpp"

#include <iostream>

#include "items/ItemManager.hpp"
#include "parser/ItemConfigParser.hpp"

int main()
{
    // 1) load global item list
    const std::string itemFile = "data/configtest/item.txt";
    ItemConfigParser  itemParser;
    if (!itemParser.ParseFromFile(itemFile))
    {
        std::cerr << "Item parse error: " << itemParser.GetLastError() << "\n";
        return 1;
    }
    ItemManager itemMgr();
    itemMgr.setItemDatabase(itemParser.GetData());

    // 2) parse player folder
    const std::string  playerDir = "data/configtest/Ghana";
    PlayerConfigParser playerParser;
    playerParser.SetItemManager(itemMgr);
    if (!playerParser.ParseFromFile(playerDir))
    {
        std::cerr << "Player parse error: " << playerParser.GetLastError() << "\n";
        return 1;
    }

    // 3) dump equipment
    std::cout << "Equipment:\n";
    for (auto& row : playerParser.GetEquipmentData())
    {
        std::cout << "  " << row[0] << " -> " << row[1] << "\n";
    }

    // 4) dump backpack
    std::cout << "\nBackpack:\n";
    const auto& bp = playerParser.GetBackpackData();
    for (size_t r = 0; r < bp.size(); ++r)
    {
        for (size_t c = 0; c < bp[r].size(); ++c)
        {
            const auto& cell = bp[r][c];
            if (cell.second > 0)
            {
                std::cout << "  (" << r << "," << c << ") " << cell.first.getItemID() << " x"
                          << cell.second << "\n";
            }
        }
    }

    // 5) dump character stats
    std::cout << "\nCharacter Stats:\n";
    for (auto& p : playerParser.GetCharStats())
    {
        std::cout << "  " << p.first << " = " << p.second << "\n";
    }

    // 6) dump unit/core stats
    std::cout << "\nUnit/Core Stats:\n";
    for (auto& p : playerParser.GetUnitStats())
    {
        std::cout << "  " << p.first << " = " << p.second << "\n";
    }

    // 7) dump type‐specific stats
    std::cout << "\nType‐Specific Stats:\n";
    for (auto& p : playerParser.GetTypeStats())
    {
        std::cout << "  " << p.first << " = " << p.second << "\n";
    }

    return 0;
}