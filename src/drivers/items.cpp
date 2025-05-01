#include <iomanip>
#include <iostream>
#include <sstream>

#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
#include "items/ItemManager.hpp"

// Function to print a horizontal line with plus signs at column boundaries
void printHorizontalLine(int columns, int width)
{
    std::cout << "+";
    for (int i = 0; i < columns; ++i)
    {
        std::cout << std::string(width, '-') << "+";
    }
    std::cout << std::endl;
}

// Function to print the backpack in a grid format
void printBackpack(const Backpack& backpack, int rows = 4, int cols = 8)
{
    std::cout << "> BACKPACK" << std::endl << std::endl;
    std::cout << "=====================[ Penyimpanan ]=====================" << std::endl;

    // Print column headers (A, B, C, ...)
    std::cout << "    ";
    for (int j = 0; j < cols; ++j)
    {
        char colLabel = 'A' + j;
        std::cout << std::setw(10) << colLabel << " ";
    }
    std::cout << std::endl;

    // Print each row
    int emptySlots = 0;
    for (int i = 0; i < rows; ++i)
    {
        printHorizontalLine(cols, 10);

        // Row number (0-based for debugging)
        std::cout << std::setw(2) << i << " |";

        // Print each column in the row
        for (int j = 0; j < cols; ++j)
        {
            try
            {
                Item item     = backpack.getItemAtTile(i, j);
                int  quantity = backpack.getQuantityAtTile(i, j);
                std::cout << " " << item.getItemID() << " (" << std::setw(2) << quantity << ") |";
            }
            catch (const EmptyCellException&)
            {
                std::cout << std::setw(10) << " |";
                emptySlots++;
            }
            catch (const std::exception& e)
            {
                std::cout << std::setw(10) << "ERROR |";
            }
        }
        std::cout << std::endl;
    }

    printHorizontalLine(cols, 10);
    std::cout << std::endl;
    std::cout << "Total slot kosong: " << emptySlots << std::endl;
}

// Function to print equipped items
void printEquippedItems(const Equipment& equipment)
{
    std::cout << "> EQUIPPED ITEM" << std::endl << std::endl;
    std::cout << "=====================[ Equipped Item ]=====================" << std::endl;

    Item weapon    = equipment.getWeapon();
    Item headArmor = equipment.getHeadArmor();
    Item bodyArmor = equipment.getBodyArmor();
    Item footArmor = equipment.getFootArmor();
    Item pendant   = equipment.getPendant();

    std::cout << "Weapon        : " << (weapon.isNull() ? "<Empty>" : weapon.getName()) << std::endl;
    std::cout << "Armor (Kepala): " << (headArmor.isNull() ? "<Empty>" : headArmor.getName())
              << std::endl;
    std::cout << "Armor (Badan) : " << (bodyArmor.isNull() ? "<Empty>" : bodyArmor.getName())
              << std::endl;
    std::cout << "Armor (Kaki)  : " << (footArmor.isNull() ? "<Empty>" : footArmor.getName())
              << std::endl;
    std::cout << "Pendant       : " << (pendant.isNull() ? "<Empty>" : pendant.getName())
              << std::endl;
}

// Function to print item details
void printItemDetails(const Item& item)
{
    if (item.isNull())
    {
        std::cout << "Empty item" << std::endl;
        return;
    }

    std::cout << item.getItemID() << " " << item.getName() << " " << item.getType() << " "
              << item.getRarity();

    // Print effects
    for (const auto& effect : item.getEffects())
    {
        std::cout << " | " << effect->GetName() << " (" << effect->GetDescription() << ")";
    }
    std::cout << std::endl;
}

// Function to print all items in the database
void printItemDatabase(const ItemManager& itemManager)
{
    std::cout << "======== ITEM DATABASE ========" << std::endl;
    for (const auto& item : itemManager.getAllItems())
    {
        printItemDetails(item);
    }
    std::cout << "===============================" << std::endl;
}

// Function to print filtered items by type
// Updated to use pointers instead of reference_wrapper
void printFilteredItems(const std::vector<std::pair<Item, int>*>& filteredItems)
{
    std::cout << "Found " << filteredItems.size() << " item(s):" << std::endl;

    for (const auto* itemPtr : filteredItems)
    {
        std::cout << "- " << itemPtr->first.getName() << " (ID: " << itemPtr->first.getItemID()
                  << ", Quantity: " << itemPtr->second << ")" << std::endl;
    }
}

int main()
{
    try
    {
        std::vector<std::shared_ptr<Effect>> effects;
        std::vector<Item>                    itemDatabase = {// Weapons
                                          Item("EXC", "Excalibur", "Weapon", 'S', effects, ""),
                                          Item("KAT", "Katana", "Weapon", 'A', effects, ""),
                                          Item("BOW", "Longbow", "Weapon", 'B', effects, ""),
                                          Item("DAG", "Dagger", "Weapon", 'C', effects, ""),
                                          Item("SWD", "Sword", "Weapon", 'D', effects, ""),

                                          // Head Armor
                                          Item("HLM", "Helmet", "HeadArmor", 'B', effects, ""),
                                          Item("HAT", "Hat", "HeadArmor", 'C', effects, ""),
                                          Item("HTL", "Heavy_Helmet", "HeadArmor", 'A', effects, ""),

                                          // Body Armor
                                          Item("BPA", "Breastplate", "BodyArmor", 'A', effects, ""),
                                          Item("LTH", "Leather_Armor", "BodyArmor", 'C', effects, ""),
                                          Item("CHN", "Chain_Mail", "BodyArmor", 'B', effects, ""),
                                          Item("BAC", "Battle_Armor", "BodyArmor", 'S', effects, ""),

                                          // Foot Armor
                                          Item("BOO", "Boots", "FootArmor", 'C', effects, ""),
                                          Item("GRV", "Greaves", "FootArmor", 'B', effects, ""),
                                          Item("SAB", "Sabatons", "FootArmor", 'A', effects, ""),

                                          // Pendants
                                          Item("AMU", "Amulet", "Pendant", 'B', effects, ""),
                                          Item("NCK", "Necklace", "Pendant", 'C', effects, ""),
                                          Item("PND", "Power_Pendant", "Pendant", 'A', effects, ""),
                                          Item("GEM", "Magic_Gem", "Pendant", 'S', effects, ""),

                                          // Potions
                                          Item("HPP", "Health_Potion", "Potion", 'C', effects, ""),
                                          Item("MPP", "Mana_Potion", "Potion", 'C', effects, ""),
                                          Item("SPP", "Strength_Potion", "Potion", 'B', effects, ""),
                                          Item("APP", "Agility_Potion", "Potion", 'B', effects, ""),
                                          Item("EPP", "Elixir_Potion", "Potion", 'A', effects, "")};

        ItemManager itemManager;
        itemManager.setItemDatabase(itemDatabase);

        std::cout << std::endl;
        std::cout << "=== Test Item and ItemManager ===" << std::endl;
        std::cout << std::endl;
        printItemDatabase(itemManager);
        std::cout << std::endl;

        std::cout << "=== Items by Type (Potion) ===" << std::endl;
        for (const auto& item : itemManager.getItemsByType("Potion"))
        {
            printItemDetails(item);
        }
        std::cout << std::endl;

        std::cout << "=== Items by Rarity (S) ===" << std::endl;
        for (const auto& item : itemManager.getItemsByRarity('S'))
        {
            printItemDetails(item);
        }
        std::cout << std::endl;

        std::vector<std::pair<Item, int>> initialItems;

        // Add some initial items to the flat vector
        initialItems.push_back({itemManager.getItem("HTL"), 2});   // Heavy Helmet
        initialItems.push_back({itemManager.getItem("BAC"), 64});  // Battle Armor (full stack)
        initialItems.push_back({itemManager.getItem("BAC"), 24});  // More Battle Armor

        // Create the Backpack with the flat vector and dimensions
        Backpack backpack(initialItems, 4, 8);

        std::cout << "=== Initial Backpack State ===" << std::endl;
        printBackpack(backpack);
        std::cout << std::endl;
        // 1. Test adding items to backpack
        std::cout << std::endl;
        std::cout << "! ===== 1. Test Adding Items to Backpack ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            // Add items to backpack
            std::cout << "Adding 5 Excalibur swords..." << std::endl;
            backpack.addItem(itemManager.getItem("EXC"), 5);

            std::cout << "Adding 3 Katanas..." << std::endl;
            backpack.addItem(itemManager.getItem("KAT"), 3);

            std::cout << "Adding 2 Amulets..." << std::endl;
            backpack.addItem(itemManager.getItem("AMU"), 2);

            std::cout << "Adding 10 Health Potions..." << std::endl;
            backpack.addItem(itemManager.getItem("HPP"), 10);

            // Print updated backpack
            std::cout << "=== Updated Backpack State ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // 2. Test adding item to a specific tile
        std::cout << "! ===== 2. Test Adding Item to Specific Tile ===== !" << std::endl;
        std::cout << std::endl;

        std::cout << std::endl;
        std::cout << "! ===== 2a. Empty Slot Addition ===== !" << std::endl;
        std::cout << std::endl;

        // Find an empty slot for the longbow using findEmptyTile
        std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();

        if (!emptyTiles.empty())
        {
            int emptyRow = emptyTiles[0].first;
            int emptyCol = emptyTiles[0].second;

            std::cout << "Adding 1 Longbow to position (" << emptyRow << ","
                      << (char)('A' + emptyCol) << ")..." << std::endl;
            backpack.addItemAtTile(emptyRow, emptyCol, itemManager.getItem("BOW"), 1);

            // Print updated backpack
            std::cout << "=== Updated Backpack State ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }
        else
        {
            std::cout << "No empty slot found for Longbow" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "! ===== 2b. Same Item Slot Addition ===== !" << std::endl;
        std::cout << std::endl;

        bool foundBow = false;
        int  bowRow = -1, bowCol = -1;

        // Find all tiles containing the Longbow item
        std::vector<std::pair<int, int>> bowTiles = backpack.findItemTile(itemManager.getItem("BO"
                                                                                              "W"));

        if (!bowTiles.empty())
        {
            bowRow   = bowTiles[0].first;
            bowCol   = bowTiles[0].second;
            foundBow = true;
        }

        if (foundBow)
        {
            std::cout << "Adding another Longbow to position (" << bowRow << ","
                      << (char)('A' + bowCol) << ")..." << std::endl;
            backpack.addItemAtTile(bowRow, bowCol, itemManager.getItem("BOW"), 1);

            // Print updated backpack
            std::cout << "=== Updated Backpack State After Adding to Same Item ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << "! ===== 2c. Different Item Slot Addition (Failure) ===== !" << std::endl;
        std::cout << "Attempting to add a Sword to a slot with a different item..." << std::endl;

        // Find a non-sword item
        for (int i = 0; i < 1; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                try
                {
                    Item item = backpack.getItemAtTile(i, j);
                    if (item.getItemID() != "SWD" && item.getItemID() != "BOW")
                    {
                        std::cout << "Trying to add a Sword to position (" << i << ","
                                  << (char)('A' + j) << ")..." << std::endl;
                        backpack.addItemAtTile(i, j, itemManager.getItem("SWD"), 1);
                        break;
                    }
                }
                catch (const ItemSlotOccupiedException& e)
                {
                    std::cout << "Correctly caught exception: " << e.what() << std::endl;
                }
            }
        }

        // 3. Test taking items from backpack
        std::cout << std::endl;
        std::cout << "! ===== 3. Test Taking Items from Backpack ===== !" << std::endl;
        try
        {
            std::cout << std::endl;
            std::cout << "! ===== 3a. Taking 2 Battle Armors ===== !" << std::endl;
            std::cout << std::endl;

            Item takenItem = backpack.takeItem(itemManager.getItem("BAC"), 2);
            std::cout << "Taken item: ";
            printItemDetails(takenItem);

            // Print updated backpack
            std::cout << "=== Updated Backpack State ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;

            std::cout << std::endl;
            std::cout << "! ===== 3b. Taking 3 Health Potions ===== !" << std::endl;
            std::cout << std::endl;

            Item takenPotion = backpack.takeItem(itemManager.getItem("HPP"), 3);
            std::cout << "Taken potion: ";
            printItemDetails(takenPotion);

            // Print updated backpack
            std::cout << "=== Updated Backpack State After Taking Potions ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // 4. Test taking item from a specific tiles
        std::cout << std::endl;
        std::cout << "! ===== 4. Test Taking Item from Specific Tile ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            // Find a slot with a HeadArmor item
            auto headArmorItems = backpack.filterItemsByType("HeadArmor");

            int helmetRow = -1, helmetCol = -1;

            if (!headArmorItems.empty())
            {
                std::vector<std::pair<int, int>> headArmorTiles =
                    backpack.findItemTile(headArmorItems[0]->first);

                if (!headArmorTiles.empty())
                {
                    helmetRow = headArmorTiles[0].first;
                    helmetCol = headArmorTiles[0].second;
                }

                std::cout << "Taking 1 item from position (" << helmetRow << ","
                          << (char)('A' + helmetCol) << ")..." << std::endl;
                Item takenItem = backpack.takeItemAtTile(helmetRow, helmetCol, 1);
                std::cout << "Taken item: ";
                printItemDetails(takenItem);

                // Print updated backpack
                std::cout << "=== Updated Backpack State ===" << std::endl;
                printBackpack(backpack);
                std::cout << std::endl;
            }
            else
            {
                std::cout << "No HeadArmor items found in the backpack" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // 5. Test moving items within backpack
        std::cout << std::endl;
        std::cout << "! ===== 5. Test Moving Items in Backpack ===== !" << std::endl;

        // Test 1: Move Battle Armor to empty slot
        std::cout << std::endl;
        std::cout << "! ===== 5a. Move Battle Armor to empty slot ===== !" << std::endl;
        std::cout << std::endl;

        int  armorRow = -1, armorCol = -1;
        bool foundArmor = false;

        // Find Battle Armor
        std::vector<std::pair<int, int>> armorTiles = backpack.findItemTile(itemManager.getItem("BA"
                                                                                                "C"));

        if (!armorTiles.empty())
        {
            armorRow   = armorTiles[0].first;
            armorCol   = armorTiles[0].second;
            foundArmor = true;
        }

        // Find an empty slot
        emptyTiles   = backpack.findEmptyTile();
        int emptyRow = -1, emptyCol = -1;

        if (!emptyTiles.empty())
        {
            emptyRow = emptyTiles[0].first;
            emptyCol = emptyTiles[0].second;
        }

        if (foundArmor && !emptyTiles.empty())
        {
            std::cout << "Moving Battle Armor from (" << armorRow << "," << (char)('A' + armorCol)
                      << ") to (" << emptyRow << "," << (char)('A' + emptyCol) << ")..."
                      << std::endl;
            backpack.moveItem(armorRow, armorCol, emptyRow, emptyCol);

            // Print updated backpack
            std::cout << "=== Updated Backpack State ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Could not find Battle Armor or empty slot for move test" << std::endl;
        }

        // Test 2: Move an item to a slot with a different item (swap)
        std::cout << std::endl;
        std::cout << "! ===== 5b. Move an item to a slot with a different item (swap) ===== !"
                  << std::endl;
        std::cout << std::endl;

        bool hasChainMail = false;
        int  chainMailRow = -1, chainMailCol = -1;

        // Find a Chain Mail item
        std::vector<std::pair<int, int>> chainMailTiles =
            backpack.findItemTile(itemManager.getItem("CHN"));

        if (!chainMailTiles.empty())
        {
            chainMailRow = chainMailTiles[0].first;
            chainMailCol = chainMailTiles[0].second;
            hasChainMail = true;
        }

        if (!hasChainMail)
        {
            // Find an empty slot for adding chain mail
            emptyTiles = backpack.findEmptyTile();
            if (!emptyTiles.empty())
            {
                chainMailRow = emptyTiles[0].first;
                chainMailCol = emptyTiles[0].second;

                std::cout << "Adding Chain Mail to position (" << chainMailRow << ","
                          << (char)('A' + chainMailCol) << ")..." << std::endl;
                backpack.addItemAtTile(chainMailRow, chainMailCol, itemManager.getItem("CHN"), 1);

                // Print updated backpack
                std::cout << "=== Backpack After Adding Chain Mail ===" << std::endl;
                printBackpack(backpack);
                std::cout << std::endl;

                hasChainMail = true;
            }
        }

        // Find a Leather Armor item or add one
        bool hasLeather = false;
        int  leatherRow = -1, leatherCol = -1;

        // Find a Leather Armor item
        std::vector<std::pair<int, int>> leatherTiles =
            backpack.findItemTile(itemManager.getItem("LTH"));

        if (!leatherTiles.empty())
        {
            leatherRow = leatherTiles[0].first;
            leatherCol = leatherTiles[0].second;
            hasLeather = true;
        }

        if (!hasLeather)
        {
            // Find an empty slot for adding leather armor
            emptyTiles = backpack.findEmptyTile();
            if (!emptyTiles.empty())
            {
                leatherRow = emptyTiles[0].first;
                leatherCol = emptyTiles[0].second;

                std::cout << "Adding Leather Armor to position (" << leatherRow << ","
                          << (char)('A' + leatherCol) << ")..." << std::endl;
                backpack.addItemAtTile(leatherRow, leatherCol, itemManager.getItem("LTH"), 1);

                // Print updated backpack
                std::cout << "=== Backpack After Adding Leather Armor ===" << std::endl;
                printBackpack(backpack);
                std::cout << std::endl;

                hasLeather = true;
            }
        }

        if (hasChainMail && hasLeather)
        {
            std::cout << "Swapping Chain Mail at (" << chainMailRow << ","
                      << (char)('A' + chainMailCol) << ") with Leather Armor at (" << leatherRow
                      << "," << (char)('A' + leatherCol) << ")..." << std::endl;
            backpack.moveItem(chainMailRow, chainMailCol, leatherRow, leatherCol);

            // Print updated backpack
            std::cout << "=== Backpack After Swapping Armors ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }

        // Test moving items to stack with same type
        // Add two swords to empty slots
        std::cout << std::endl;
        std::cout << "! ===== 5c. Test moving items to stack with same type ===== !" << std::endl;
        std::cout << std::endl;

        int sword1Row = -1, sword1Col = -1;
        int sword2Row = -1, sword2Col = -1;

        // Find empty slots for swords
        emptyTiles = backpack.findEmptyTile();
        if (emptyTiles.size() >= 2)
        {
            sword1Row = emptyTiles[0].first;
            sword1Col = emptyTiles[0].second;
            sword2Row = emptyTiles[1].first;
            sword2Col = emptyTiles[1].second;

            backpack.addItemAtTile(sword1Row, sword1Col, itemManager.getItem("SWD"), 1);
            std::cout << "Added 1 sword to position (" << sword1Row << ","
                      << (char)('A' + sword1Col) << ")" << std::endl;

            backpack.addItemAtTile(sword2Row, sword2Col, itemManager.getItem("SWD"), 1);
            std::cout << "Added 1 sword to position (" << sword2Row << ","
                      << (char)('A' + sword2Col) << ")" << std::endl;

            // Print updated backpack
            std::cout << "=== Backpack After Adding Swords ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;

            // Move sword1 to sword2 position (should stack)
            std::cout << "Moving sword from (" << sword1Row << "," << (char)('A' + sword1Col)
                      << ") to (" << sword2Row << "," << (char)('A' + sword2Col) << ")..."
                      << std::endl;
            backpack.moveItem(sword1Row, sword1Col, sword2Row, sword2Col);

            // Print updated backpack
            std::cout << "=== Backpack After Moving Swords (Stacking) ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }

        // 6. Test the filterItemsByType method
        std::cout << std::endl;
        std::cout << "! ===== 6. Test Filtering Items by Type ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            // Add more items of different types to ensure we have variety
            // Add some potions
            std::cout << "Adding different potions for testing filter..." << std::endl;
            backpack.addItem(itemManager.getItem("MPP"), 5);  // Mana potions
            backpack.addItem(itemManager.getItem("SPP"), 2);  // Strength potions

            // Filter for potions
            std::cout << "Filtering for Potions:" << std::endl;
            auto potions = backpack.filterItemsByType("Potion");
            printFilteredItems(potions);

            // Test modifying potions through the filtered results
            std::cout << "\n! ===== 6a. Test Modifying Items Through Filtered Results ===== !"
                      << std::endl;
            if (!potions.empty())
            {
                // get the first potion pair using pointer
                auto* firstPotionPair  = potions[0];
                int   originalQuantity = firstPotionPair->second;

                std::cout << "Original potion quantity: " << originalQuantity << std::endl;
                std::cout << "Modifying quantity through filtered result pointer..." << std::endl;

                // Modify the quantity through the pointer
                firstPotionPair->second += 10;

                std::cout << "Modified potion quantity: " << firstPotionPair->second << std::endl;

                // Now check if the backpack reflects the change
                std::vector<std::pair<int, int>> potionTiles =
                    backpack.findItemTile(firstPotionPair->first);
                if (!potionTiles.empty())
                {
                    int row              = potionTiles[0].first;
                    int col              = potionTiles[0].second;
                    int backpackQuantity = backpack.getQuantityAtTile(row, col);

                    std::cout << "Quantity in backpack at position (" << row << ","
                              << (char)('A' + col) << "): " << backpackQuantity << std::endl;

                    if (backpackQuantity == firstPotionPair->second)
                    {
                        std::cout << "SUCCESS: Backpack quantity matches the modified quantity!"
                                  << std::endl;
                    }
                    else
                    {
                        std::cout << "ERROR: Backpack quantity does not match the modified "
                                     "quantity!"
                                  << std::endl;
                    }
                }

                // Restore original quantity for other tests
                firstPotionPair->second = originalQuantity;
            }
            else
            {
                std::cout << "No potions found to test modification" << std::endl;
            }

            // Filter for weapons
            std::cout << "\nFiltering for Weapons:" << std::endl;
            auto weapons = backpack.filterItemsByType("Weapon");
            printFilteredItems(weapons);

            // Filter for body armor
            std::cout << "\nFiltering for Body Armor:" << std::endl;
            auto bodyArmor = backpack.filterItemsByType("BodyArmor");
            printFilteredItems(bodyArmor);

            // Filter for type that doesn't exist
            std::cout << "\nFiltering for non-existent type (Food):" << std::endl;
            auto nonExistent = backpack.filterItemsByType("Food");
            printFilteredItems(nonExistent);

            std::cout << "=== Backpack After Adding Items for Filtering ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        Equipment equipment;

        std::cout << "=== Initial Equipment State ===" << std::endl;
        printEquippedItems(equipment);
        std::cout << std::endl;

        // 7. Test equipping items from backpack
        std::cout << std::endl;
        std::cout << "! ===== 7. Test equipping items from backpack ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            // Add various equipment items to backpack for testing
            // Find empty slots for each type of equipment
            std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();

            if (emptyTiles.size() >= 4)
            {
                // Add boots
                int bootsRow = emptyTiles[0].first;
                int bootsCol = emptyTiles[0].second;
                std::cout << "Adding boots to position (" << bootsRow << ","
                          << (char)('A' + bootsCol) << ")..." << std::endl;
                backpack.addItemAtTile(bootsRow, bootsCol, itemManager.getItem("BOO"), 1);

                // Add sword
                int swordRow = emptyTiles[1].first;
                int swordCol = emptyTiles[1].second;
                std::cout << "Adding sword to position (" << swordRow << ","
                          << (char)('A' + swordCol) << ")..." << std::endl;
                backpack.addItemAtTile(swordRow, swordCol, itemManager.getItem("SWD"), 1);

                // Add helmet
                int helmetRow = emptyTiles[2].first;
                int helmetCol = emptyTiles[2].second;
                std::cout << "Adding helmet to position (" << helmetRow << ","
                          << (char)('A' + helmetCol) << ")..." << std::endl;
                backpack.addItemAtTile(helmetRow, helmetCol, itemManager.getItem("HLM"), 1);

                // Add pendant
                int pendantRow = emptyTiles[3].first;
                int pendantCol = emptyTiles[3].second;
                std::cout << "Adding pendant to position (" << pendantRow << ","
                          << (char)('A' + pendantCol) << ")..." << std::endl;
                backpack.addItemAtTile(pendantRow, pendantCol, itemManager.getItem("NCK"), 1);
            }
            else
            {
                std::cout << "Not enough empty slots for equipment items" << std::endl;
            }

            // Print the backpack before equipping
            std::cout << "=== Backpack Before Equipping ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;

            int count = 0;

            // Find and equip weapon
            auto weapons = backpack.filterItemsByType("Weapon");

            for (auto* weaponPtr : weapons)
            {
                Item& item = weaponPtr->first;

                if (count < 2)
                {
                    std::vector<std::pair<int, int>> weaponTiles = backpack.findItemTile(item);

                    if (!weaponTiles.empty())
                    {
                        int weaponRow = weaponTiles[0].first;
                        int weaponCol = weaponTiles[0].second;

                        std::cout << "Equipping weapon from position (" << weaponRow << ","
                                  << (char)('A' + weaponCol) << ")..." << std::endl;
                        equipment.equipItemFromBackpack(backpack, weaponRow, weaponCol, "Weapon");
                        std::cout << "Weapon equipped" << std::endl;
                        printEquippedItems(equipment);
                        printBackpack(backpack);
                    }
                }
                count++;
            }

            count = 0;
            // Find and equip head armor
            auto headArmors = backpack.filterItemsByType("HeadArmor");

            for (auto* headArmorPtr : headArmors)
            {
                Item& item = headArmorPtr->first;

                if (count < 2)
                {
                    std::vector<std::pair<int, int>> headArmorTiles = backpack.findItemTile(item);

                    if (!headArmorTiles.empty())
                    {
                        int headArmorRow = headArmorTiles[0].first;
                        int headArmorCol = headArmorTiles[0].second;

                        std::cout << "Equipping head armor from position (" << headArmorRow << ","
                                  << (char)('A' + headArmorCol) << ")..." << std::endl;
                        equipment.equipItemFromBackpack(
                            backpack, headArmorRow, headArmorCol, "HeadArmor");
                        std::cout << "Head armor equipped" << std::endl;
                        printEquippedItems(equipment);
                        printBackpack(backpack);
                    }
                }
                count++;
            }

            count = 0;
            // Find and equip body armor
            auto bodyArmors = backpack.filterItemsByType("BodyArmor");

            for (auto* bodyArmorPtr : bodyArmors)
            {
                Item& item = bodyArmorPtr->first;

                if (count < 2)
                {
                    std::vector<std::pair<int, int>> bodyArmorTiles = backpack.findItemTile(item);

                    if (!bodyArmorTiles.empty())
                    {
                        int bodyArmorRow = bodyArmorTiles[0].first;
                        int bodyArmorCol = bodyArmorTiles[0].second;

                        std::cout << "Equipping body armor from position (" << bodyArmorRow << ","
                                  << (char)('A' + bodyArmorCol) << ")..." << std::endl;
                        equipment.equipItemFromBackpack(
                            backpack, bodyArmorRow, bodyArmorCol, "BodyArmor");
                        std::cout << "Body armor equipped" << std::endl;
                        printEquippedItems(equipment);
                        printBackpack(backpack);
                    }
                }
                count++;
            }

            count = 0;
            // Find and equip foot armor
            auto footArmors = backpack.filterItemsByType("FootArmor");

            for (auto* footArmorPtr : footArmors)
            {
                Item& item = footArmorPtr->first;

                if (count < 2)
                {
                    std::vector<std::pair<int, int>> footArmorTiles = backpack.findItemTile(item);

                    if (!footArmorTiles.empty())
                    {
                        int footArmorRow = footArmorTiles[0].first;
                        int footArmorCol = footArmorTiles[0].second;

                        std::cout << "Equipping foot armor from position (" << footArmorRow << ","
                                  << (char)('A' + footArmorCol) << ")..." << std::endl;
                        equipment.equipItemFromBackpack(
                            backpack, footArmorRow, footArmorCol, "FootArmor");
                        std::cout << "Foot armor equipped" << std::endl;
                        printEquippedItems(equipment);
                        printBackpack(backpack);
                    }
                }
                count++;
            }

            count = 0;
            // Find and equip pendant
            auto pendants = backpack.filterItemsByType("Pendant");

            for (auto* pendantPtr : pendants)
            {
                Item& item = pendantPtr->first;

                if (count < 2)
                {
                    std::vector<std::pair<int, int>> pendantTiles = backpack.findItemTile(item);

                    if (!pendantTiles.empty())
                    {
                        int pendantRow = pendantTiles[0].first;
                        int pendantCol = pendantTiles[0].second;

                        std::cout << "Equipping pendant from position (" << pendantRow << ","
                                  << (char)('A' + pendantCol) << ")..." << std::endl;
                        equipment.equipItemFromBackpack(backpack, pendantRow, pendantCol, "Pendant");
                        std::cout << "Pendant equipped" << std::endl;
                        printEquippedItems(equipment);
                        printBackpack(backpack);
                    }
                }
                count++;
            }

            // Print equipment after equipping
            std::cout << "=== Equipment After Equipping ===" << std::endl;
            printEquippedItems(equipment);
            std::cout << std::endl;

            // Print backpack after equipping
            std::cout << "=== Backpack After Equipping ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // 8. Test unequipping items to backpack
        std::cout << std::endl;
        std::cout << "! ===== 8. Test unequipping items from backpack ===== !" << std::endl;
        std::cout << std::endl;
        try
        {
            // Test 1: Unequip to empty slot
            std::cout << std::endl;
            std::cout << "! ===== 8a. Unequip to empty slot ===== !" << std::endl;
            std::cout << std::endl;

            // Find empty slot for the weapon
            std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();
            if (!emptyTiles.empty())
            {
                int emptyWeaponRow = emptyTiles[0].first;
                int emptyWeaponCol = emptyTiles[0].second;

                std::cout << "Unequipping weapon to empty position (" << emptyWeaponRow << ","
                          << (char)('A' + emptyWeaponCol) << ")..." << std::endl;
                equipment.unequipItemToBackpack(backpack, emptyWeaponRow, emptyWeaponCol, "Weapon");
                std::cout << "Weapon unequipped" << std::endl;
            }
            else
            {
                std::cout << "No empty slot found for unequipping weapon" << std::endl;
            }

            printEquippedItems(equipment);
            printBackpack(backpack);

            // Test 2: Unequip to slot with same type (should swap)
            std::cout << std::endl;
            std::cout << "! ===== 8b. Unequip to slot with same type (should swap) ===== !"
                      << std::endl;
            std::cout << std::endl;

            // Add another weapon to backpack
            emptyTiles = backpack.findEmptyTile();
            if (!emptyTiles.empty())
            {
                int secondWeaponRow = emptyTiles[0].first;
                int secondWeaponCol = emptyTiles[0].second;

                std::cout << "Adding another weapon (Katana) to position (" << secondWeaponRow
                          << "," << (char)('A' + secondWeaponCol) << ")..." << std::endl;
                backpack.addItemAtTile(
                    secondWeaponRow, secondWeaponCol, itemManager.getItem("KAT"), 1);

                printBackpack(backpack);

                // Equip a sword if we can find one in the backpack
                Item                             swordItem     = itemManager.getItem("SWD");
                std::vector<std::pair<int, int>> swordTiles    = backpack.findItemTile(swordItem);
                bool                             swordEquipped = false;

                if (!swordTiles.empty())
                {
                    int swordRow = swordTiles[0].first;
                    int swordCol = swordTiles[0].second;

                    std::cout << "Equipping a sword as current weapon..." << std::endl;
                    equipment.equipItemFromBackpack(backpack, swordRow, swordCol, "Weapon");
                    swordEquipped = true;
                }
                else
                {
                    // If no sword found, add one to an empty slot and equip it
                    emptyTiles = backpack.findEmptyTile();
                    if (!emptyTiles.empty())
                    {
                        int swordRow = emptyTiles[0].first;
                        int swordCol = emptyTiles[0].second;

                        backpack.addItemAtTile(swordRow, swordCol, itemManager.getItem("SWD"), 1);
                        equipment.equipItemFromBackpack(backpack, swordRow, swordCol, "Weapon");
                        swordEquipped = true;
                    }
                }

                if (swordEquipped)
                {
                    // Print current equipment
                    std::cout << "=== Current Equipment ===" << std::endl;
                    printEquippedItems(equipment);
                    printBackpack(backpack);
                    std::cout << std::endl;

                    // Find the Katana position again as it might have changed
                    swordItem  = itemManager.getItem("KAT");
                    swordTiles = backpack.findItemTile(swordItem);

                    if (!swordTiles.empty())
                    {
                        secondWeaponRow = swordTiles[0].first;
                        secondWeaponCol = swordTiles[0].second;

                        // Now try to unequip to the slot with Katana (should swap)
                        std::cout << "Unequipping weapon to position with same type ("
                                  << secondWeaponRow << "," << (char)('A' + secondWeaponCol)
                                  << ")..." << std::endl;
                        equipment.unequipItemToBackpack(
                            backpack, secondWeaponRow, secondWeaponCol, "Weapon");
                        std::cout << "Weapon swapped" << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "No empty slot found for testing weapon swap" << std::endl;
            }

            printEquippedItems(equipment);
            printBackpack(backpack);

            // Test 3: Unequip to slot with different type (should use empty slot)
            std::cout << std::endl;
            std::cout << "! ===== 8c. Unequip to slot with different type (should use empty slot) "
                         "===== !"
                      << std::endl;
            std::cout << std::endl;

            // Find a slot with a potion
            auto potions = backpack.filterItemsByType("Potion");

            if (!potions.empty())
            {
                std::vector<std::pair<int, int>> potionTiles =
                    backpack.findItemTile(potions[0]->first);

                if (!potionTiles.empty())
                {
                    int potionRow = potionTiles[0].first;
                    int potionCol = potionTiles[0].second;

                    std::cout << "Unequipping body armor to position with potion (" << potionRow
                              << "," << (char)('A' + potionCol) << ")..." << std::endl;
                    equipment.unequipItemToBackpack(backpack, potionRow, potionCol, "BodyArmor");
                    std::cout << "Body armor unequipped" << std::endl;
                }
            }
            else
            {
                std::cout << "No potions found in the backpack" << std::endl;
            }

            printEquippedItems(equipment);
            printBackpack(backpack);

            // Test 4: Unequip to slot with multiple items (stack)
            std::cout << std::endl;
            std::cout << "! ===== 8d. Unequip to slot with multiple items (stack) ===== !"
                      << std::endl;
            std::cout << std::endl;

            // Add boots
            backpack.addItem(itemManager.getItem("BOO"), 2);
            printBackpack(backpack);

            equipment.unequipItemToBackpack(backpack, 0, 0, "FootArmor");

            printEquippedItems(equipment);
            printBackpack(backpack);
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // 9. Test error handling
        std::cout << std::endl;
        std::cout << "! ===== 9. Test Error Handling ===== !" << std::endl;
        std::cout << std::endl;

        // Test invalid coordinates
        std::cout << std::endl;
        std::cout << "! ===== 9a. Test invalid coordinates ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            std::cout << "Testing invalid coordinates (10, 5)..." << std::endl;
            backpack.getItemAtTile(10, 5);
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // Test empty cell
        std::cout << std::endl;
        std::cout << "! ===== 9b. Test empty cell ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            std::cout << "Testing empty cell access at (0, 0)..." << std::endl;
            // Find an empty cell first
            std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();
            if (!emptyTiles.empty())
            {
                backpack.takeItemAtTile(emptyTiles[0].first, emptyTiles[0].second, 1);
            }
            else
            {
                std::cout << "No empty cells to test" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // Test insufficient quantity
        std::cout << std::endl;
        std::cout << "! ===== 9c. Test insufficient quantity ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            std::cout << "Testing taking more items than available..." << std::endl;
            backpack.takeItem(itemManager.getItem("EXC"), 1000);
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // Test item slot occupied
        std::cout << std::endl;
        std::cout << "! ===== 9d. Test item slot occupied ===== !" << std::endl;
        std::cout << std::endl;

        std::cout << "Testing adding different item to occupied slot..." << std::endl;

        try
        {
            // Find a slot with a Weapon
            auto weapons = backpack.filterItemsByType("Weapon");

            if (!weapons.empty())
            {
                std::vector<std::pair<int, int>> weaponTiles =
                    backpack.findItemTile(weapons[0]->first);

                if (!weaponTiles.empty())
                {
                    int weaponRow = weaponTiles[0].first;
                    int weaponCol = weaponTiles[0].second;

                    std::cout << "Trying to add a Helmet to position (" << weaponRow << ","
                              << (char)('A' + weaponCol) << ") that has a Weapon..." << std::endl;
                    backpack.addItemAtTile(weaponRow, weaponCol, itemManager.getItem("HLM"), 1);
                }
            }
            else
            {
                std::cout << "No weapons found in the backpack" << std::endl;
            }
        }
        catch (const ItemSlotOccupiedException& e)
        {
            std::cout << "Correctly caught ItemSlotOccupiedException: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught unexpected exception: " << e.what() << std::endl;
        }

        // Test backpack full exception
        std::cout << std::endl;
        std::cout << "! ===== 9e. Test backpack full exception ===== !" << std::endl;
        std::cout << std::endl;

        try
        {
            std::cout << "Testing backpack full exception..." << std::endl;

            // First, fill all remaining empty slots
            std::vector<std::pair<int, int>> emptyTiles  = backpack.findEmptyTile();
            int                              filledCount = 0;

            for (const auto& [x, y] : emptyTiles)
            {
                try
                {
                    backpack.addItemAtTile(x, y, itemManager.getItem("HPP"), 1);
                    filledCount++;
                }
                catch (const std::exception& e)
                {
                    std::cout << "Caught exception: " << e.what() << std::endl;
                }
            }

            std::cout << "Filled " << filledCount << " empty slots with Health Potions" << std::endl;

            // Print backpack after filling
            std::cout << "=== Backpack After Filling Empty Slots ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;

            // Now try to add more items than the backpack can hold
            std::cout << "Attempting to add 100 more items to a full backpack..." << std::endl;
            backpack.addItem(itemManager.getItem("EPP"), 100);
        }
        catch (const BackpackOvercapacityException& e)
        {
            std::cout << "Correctly caught BackpackOvercapacityException: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught unexpected exception: " << e.what() << std::endl;
        }

        std::cout << std::endl;
        std::cout << "=== All Tests Completed ===" << std::endl;
        std::cout << std::endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unhandled unknown exception" << std::endl;
        return 1;
    }
}