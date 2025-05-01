#include <iostream>

#include "items/Item.hpp"
#include "parser/ItemConfigParser.hpp"

int main()
{
    const std::string filename = "data/configtest/item.txt";
    ItemConfigParser  parser;

    if (!parser.ParseFromFile(filename))
    {
        std::cerr << "Parse error: " << parser.GetLastError() << "\n";
        return 1;
    }

    const auto& items = parser.GetData();
    std::cout << "Parsed " << items.size() << " items:\n";

    for (const auto& item : items)
    {
        std::cout << "ID=" << item.getItemID() << "  Name=" << item.getName()
                  << "  Type=" << item.getType() << "  Rarity=" << item.getRarity() << "  Effects=[";
        for (const auto& e : item.getEffects())
            std::cout << e << ",";
        std::cout << "]\n";
        std::cout << "Description: " << item.getDescription() << "\n";
    }

    return 0;
}