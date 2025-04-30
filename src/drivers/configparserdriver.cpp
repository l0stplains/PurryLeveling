#include <iostream>
#include <memory>
#include <vector>

#include "parser/BackpackConfigParser.hpp"
#include "parser/InventoryConfigParser.hpp"
#include "parser/ItemConfigParser.hpp"
#include "parser/MobLootConfigParser.hpp"
#include "parser/ShopConfigParser.hpp"

int main()
{
    struct TestCase
    {
        std::string                   name;
        std::string                   filename;
        std::unique_ptr<ConfigParser> parser;
    };

    std::vector<TestCase> tests;
    tests.push_back({"ItemConfig", "data/configtest/item.txt", std::make_unique<ItemConfigParser>()});
    tests.push_back({"ShopConfig", "data/configtest/shop.txt", std::make_unique<ShopConfigParser>()});
    tests.push_back(
        {"BackpackConfig", "data/configtest/backpack.txt", std::make_unique<BackpackConfigParser>()});
    tests.push_back(
        {"MobLootConfig", "data/configtest/mobloot.txt", std::make_unique<MobLootConfigParser>()});
    tests.push_back({"InventoryConfig",
                     "data/configtest/equipment.txt",
                     std::make_unique<InventoryConfigParser>()});

    for (auto& tc : tests)
    {
        std::cout << "--- Testing " << tc.name << " ---\n";
        if (!tc.parser->ParseFromFile(tc.filename))
        {
            std::cerr << "ERROR parsing " << tc.filename << ": " << tc.parser->GetLastError() << "\n";
            continue;
        }

        // Downcast to access GetData()
        // All parsers provide GetData() returning vector<vector<string>>
        auto* generic = dynamic_cast<ConfigParser*>(tc.parser.get());
        // We'll use reflection-like hack: include each specific type
        if (tc.name == "ItemConfig")
        {
            auto* p = static_cast<ItemConfigParser*>(tc.parser.get());
            for (auto& row : p->GetData())
            {
                for (auto& tok : row)
                    std::cout << tok << " ";
                std::cout << "\n";
            }
        }
        else if (tc.name == "ShopConfig")
        {
            auto* p = static_cast<ShopConfigParser*>(tc.parser.get());
            for (auto& row : p->GetData())
            {
                for (auto& tok : row)
                    std::cout << tok << " ";
                std::cout << "\n";
            }
        }
        else if (tc.name == "BackpackConfig")
        {
            auto* p = static_cast<BackpackConfigParser*>(tc.parser.get());
            for (auto& row : p->GetData())
            {
                for (auto& tok : row)
                    std::cout << tok << " ";
                std::cout << "\n";
            }
        }
        else if (tc.name == "MobLootConfig")
        {
            auto* p = static_cast<MobLootConfigParser*>(tc.parser.get());
            for (auto& row : p->GetData())
            {
                for (auto& tok : row)
                    std::cout << tok << " ";
                std::cout << "\n";
            }
        }
        else if (tc.name == "InventoryConfig")
        {
            auto* p = static_cast<InventoryConfigParser*>(tc.parser.get());
            for (auto& row : p->GetData())
            {
                for (auto& tok : row)
                    std::cout << tok << " ";
                std::cout << "\n";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}