#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "exception/Exception.hpp"
#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
#include "items/Item.hpp"
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
        std::cout << " " << effect;
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
void printFilteredItems(const std::vector<std::pair<Item, int>>& filteredItems)
{
    std::cout << "Found " << filteredItems.size() << " item(s):" << std::endl;

    for (const auto& pair : filteredItems)
    {
        std::cout << "- " << pair.first.getName() << " (ID: " << pair.first.getItemID()
                  << ", Quantity: " << pair.second << ")" << std::endl;
    }
}

// Helper function to find an empty slot in the backpack
bool findEmptySlot(const Backpack& backpack, int& outRow, int& outCol)
{
    for (int i = 0; i < 4; ++i)
    {  // MODIFIED: Changed from 8 to 4 rows
        for (int j = 0; j < 8; ++j)
        {  // MODIFIED: Changed from 4 to 8 columns
            try
            {
                backpack.getItemAtTile(i, j);
            }
            catch (const EmptyCellException&)
            {
                // Found an empty slot
                outRow = i;
                outCol = j;
                return true;
            }
            catch (...)
            {
                // Skip other exceptions
            }
        }
    }
    return false;  // No empty slot found
}

// Helper function to find a slot with a specific item type
bool findItemTypeSlot(const Backpack& backpack, const std::string& itemType, int& outRow, int& outCol)
{
    for (int i = 0; i < 4; ++i)
    {  // MODIFIED: Changed from 8 to 4 rows
        for (int j = 0; j < 8; ++j)
        {  // MODIFIED: Changed from 4 to 8 columns
            try
            {
                Item item = backpack.getItemAtTile(i, j);
                if (item.getType() == itemType)
                {
                    outRow = i;
                    outCol = j;
                    return true;
                }
            }
            catch (...)
            {
                // Skip exceptions
            }
        }
    }
    return false;  // No slot with the specified item type found
}

// Helper function to find a slot with a specific item ID
bool findItemIdSlot(const Backpack& backpack, const std::string& itemId, int& outRow, int& outCol)
{
    for (int i = 0; i < 4; ++i)
    {  // MODIFIED: Changed from 8 to 4 rows
        for (int j = 0; j < 8; ++j)
        {  // MODIFIED: Changed from 4 to 8 columns
            try
            {
                Item item = backpack.getItemAtTile(i, j);
                if (item.getItemID() == itemId)
                {
                    outRow = i;
                    outCol = j;
                    return true;
                }
            }
            catch (...)
            {
                // Skip exceptions
            }
        }
    }
    return false;  // No slot with the specified item ID found
}

// Helper function to add an item to the backpack, finding an appropriate slot
bool addItemToBackpack(Backpack& backpack, const Item& item, int quantity, int& outRow, int& outCol)
{
    // First try to find a slot with the same item type
    for (int i = 0; i < 4; ++i)
    {  // MODIFIED: Changed from 8 to 4 rows
        for (int j = 0; j < 8; ++j)
        {  // MODIFIED: Changed from 4 to 8 columns
            try
            {
                Item currentItem = backpack.getItemAtTile(i, j);
                if (currentItem == item)
                {
                    try
                    {
                        backpack.addItemAtTile(i, j, item, quantity);
                        outRow = i;
                        outCol = j;
                        return true;
                    }
                    catch (...)
                    {
                        // Stack might be full, continue looking
                    }
                }
            }
            catch (...)
            {
                // Skip exceptions
            }
        }
    }

    // Then try to find an empty slot
    for (int i = 0; i < 4; ++i)
    {  // MODIFIED: Changed from 8 to 4 rows
        for (int j = 0; j < 8; ++j)
        {  // MODIFIED: Changed from 4 to 8 columns
            try
            {
                backpack.getItemAtTile(i, j);
            }
            catch (const EmptyCellException&)
            {
                // Found an empty slot
                try
                {
                    backpack.addItemAtTile(i, j, item, quantity);
                    outRow = i;
                    outCol = j;
                    return true;
                }
                catch (...)
                {
                    // Something went wrong, try another slot
                }
            }
            catch (...)
            {
                // Skip other exceptions
            }
        }
    }

    // Last resort, try to use addItem which will find a suitable slot
    try
    {
        backpack.addItem(item, quantity);
        return true;  // Can't set outRow and outCol here
    }
    catch (...)
    {
        return false;  // Couldn't add the item
    }
}

int main()
{
    try
    {
        // 1. Create item database with added Potion items
        std::vector<Item> itemDatabase = {
            // Weapons
            Item("EXC", "Excalibur", "Weapon", 'S', {"500", "Critical_Strike"}),
            Item("KAT", "Katana", "Weapon", 'A', {"300", "Quick_Attack"}),
            Item("BOW", "Longbow", "Weapon", 'B', {"250", "Range_Attack"}),
            Item("DAG", "Dagger", "Weapon", 'C', {"150", "Stealth_Attack"}),
            Item("SWD", "Sword", "Weapon", 'D', {"100", "Basic_Attack"}),

            // Head Armor
            Item("HLM", "Helmet", "HeadArmor", 'B', {"50", "Head_Protection"}),
            Item("HAT", "Hat", "HeadArmor", 'C', {"20", "Style_Points"}),
            Item("HTL", "Heavy_Helmet", "HeadArmor", 'A', {"100", "Strong_Defense"}),

            // Body Armor
            Item("BPA", "Breastplate", "BodyArmor", 'A', {"200", "Body_Protection"}),
            Item("LTH", "Leather_Armor", "BodyArmor", 'C', {"80", "Light_Weight"}),
            Item("CHN", "Chain_Mail", "BodyArmor", 'B', {"150", "Medium_Protection"}),
            Item("BAC", "Battle_Armor", "BodyArmor", 'S', {"300", "Full_Protection"}),

            // Foot Armor
            Item("BOO", "Boots", "FootArmor", 'C', {"30", "Basic_Protection"}),
            Item("GRV", "Greaves", "FootArmor", 'B', {"50", "Enhanced_Movement"}),
            Item("SAB", "Sabatons", "FootArmor", 'A', {"80", "Heavy_Protection"}),

            // Pendants
            Item("AMU", "Amulet", "Pendant", 'B', {"Magic_Boost"}),
            Item("NCK", "Necklace", "Pendant", 'C', {"Charm"}),
            Item("PND", "Power_Pendant", "Pendant", 'A', {"Strength_Boost"}),
            Item("GEM", "Magic_Gem", "Pendant", 'S', {"Special_Power"}),

            // Potions
            Item("HPP", "Health_Potion", "Potion", 'C', {"Heal_50"}),
            Item("MPP", "Mana_Potion", "Potion", 'C', {"Mana_50"}),
            Item("SPP", "Strength_Potion", "Potion", 'B', {"Strength_Boost"}),
            Item("APP", "Agility_Potion", "Potion", 'B', {"Agility_Boost"}),
            Item("EPP", "Elixir_Potion", "Potion", 'A', {"Full_Restore"})};

        // 2. Create ItemManager
        ItemManager itemManager(itemDatabase);

        // 3. Print item database
        std::cout << "=== Test Item and ItemManager ===" << std::endl;
        printItemDatabase(itemManager);
        std::cout << std::endl;

        // 4. Test Item type filtering
        std::cout << "=== Items by Type (Potion) ===" << std::endl;
        for (const auto& item : itemManager.getItemsByType("Potion"))
        {
            printItemDetails(item);
        }
        std::cout << std::endl;

        // 5. Test Item rarity filtering
        std::cout << "=== Items by Rarity (S) ===" << std::endl;
        for (const auto& item : itemManager.getItemsByRarity('S'))
        {
            printItemDetails(item);
        }
        std::cout << std::endl;

        // 6. Create and initialize a backpack with some items
        // MODIFIED: Use the new constructor with flat vector of (Item, quantity) pairs
        std::vector<std::pair<Item, int>> initialItems;

        // Add some initial items to the flat vector
        initialItems.push_back({itemManager.getItem("HTL"), 2});   // Heavy Helmet
        initialItems.push_back({itemManager.getItem("BAC"), 64});  // Battle Armor (full stack)
        initialItems.push_back({itemManager.getItem("BAC"), 24});  // More Battle Armor

        // Create the Backpack with the flat vector and dimensions
        // MODIFIED: Changed dimensions to 4 rows x 8 columns
        Backpack backpack(initialItems, 4, 8);

        // 7. Print initial backpack state
        std::cout << "=== Initial Backpack State ===" << std::endl;
        printBackpack(backpack);
        std::cout << std::endl;

        // 8. Test adding items to backpack
        std::cout << "=== Test Adding Items to Backpack ===" << std::endl;
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
            std::cout << "Error: " << e.what() << std::endl;
        }

        // 9. Test adding item to a specific tile
        std::cout << "=== Test Adding Item to Specific Tile ===" << std::endl;
        try
        {
            // Find an empty slot for the longbow
            int emptyRow = -1, emptyCol = -1;
            if (findEmptySlot(backpack, emptyRow, emptyCol))
            {
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

            // Test adding to a slot with the same item (success case)
            int  bowRow = -1, bowCol = -1;
            bool foundBow = false;
            for (int i = 0; i < 4; ++i)
            {  // MODIFIED: Changed from 8 to 4 rows
                for (int j = 0; j < 8; ++j)
                {  // MODIFIED: Changed from 4 to 8 columns
                    try
                    {
                        Item item = backpack.getItemAtTile(i, j);
                        if (item.getItemID() == "BOW")
                        {
                            bowRow   = i;
                            bowCol   = j;
                            foundBow = true;
                            break;
                        }
                    }
                    catch (...)
                    {
                        // Skip exceptions
                    }
                }
                if (foundBow)
                    break;
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

            // Test adding to a slot with a different item (failure case)
            std::cout << "Attempting to add a Sword to a slot with a different item..." << std::endl;
            try
            {
                // Find a slot with a non-sword item
                int nonSwordRow = -1, nonSwordCol = -1;
                for (int i = 0; i < 4; ++i)
                {  // MODIFIED: Changed from 8 to 4 rows
                    for (int j = 0; j < 8; ++j)
                    {  // MODIFIED: Changed from 4 to 8 columns
                        try
                        {
                            Item item = backpack.getItemAtTile(i, j);
                            if (item.getItemID() != "SWD" && item.getItemID() != "BOW")
                            {
                                nonSwordRow = i;
                                nonSwordCol = j;
                                break;
                            }
                        }
                        catch (...)
                        {
                            // Skip exceptions
                        }
                    }
                    if (nonSwordRow != -1)
                        break;
                }

                if (nonSwordRow != -1)
                {
                    std::cout << "Trying to add a Sword to position (" << nonSwordRow << ","
                              << (char)('A' + nonSwordCol) << ")..." << std::endl;
                    backpack.addItemAtTile(nonSwordRow, nonSwordCol, itemManager.getItem("SWD"), 1);
                }
            }
            catch (const ItemSlotOccupiedException& e)
            {
                std::cout << "Correctly caught exception: " << e.what() << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }

        // 10. Test taking items from backpack
        std::cout << "=== Test Taking Items from Backpack ===" << std::endl;
        try
        {
            std::cout << "Taking 2 Battle Armors..." << std::endl;
            Item takenItem = backpack.takeItem(itemManager.getItem("BAC"), 2);
            std::cout << "Taken item: ";
            printItemDetails(takenItem);

            // Print updated backpack
            std::cout << "=== Updated Backpack State ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;

            // Take a potion
            std::cout << "Taking 3 Health Potions..." << std::endl;
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
            std::cout << "Error: " << e.what() << std::endl;
        }

        // 11. Test taking item from a specific tile
        std::cout << "=== Test Taking Item from Specific Tile ===" << std::endl;
        try
        {
            // Find a slot with the helmet
            int  helmetRow = -1, helmetCol = -1;
            bool foundHelmet = false;
            for (int i = 0; i < 4; ++i)
            {  // MODIFIED: Changed from 8 to 4 rows
                for (int j = 0; j < 8; ++j)
                {  // MODIFIED: Changed from 4 to 8 columns
                    try
                    {
                        Item item = backpack.getItemAtTile(i, j);
                        if (item.getType() == "HeadArmor")
                        {
                            helmetRow   = i;
                            helmetCol   = j;
                            foundHelmet = true;
                            break;
                        }
                    }
                    catch (...)
                    {
                        // Skip exceptions
                    }
                }
                if (foundHelmet)
                    break;
            }

            if (foundHelmet)
            {
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
                std::cout << "Could not find a helmet to take" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }

        // 12. Test moving items within backpack
        std::cout << "=== Test Moving Items in Backpack ===" << std::endl;
        try
        {
            // Test 1: Move Battle Armor to empty slot
            int armorRow = -1, armorCol = -1;
            int emptyRow = -1, emptyCol = -1;

            // Find Battle Armor slot
            for (int i = 0; i < 4; ++i)
            {  // MODIFIED: Changed from 8 to 4 rows
                for (int j = 0; j < 8; ++j)
                {  // MODIFIED: Changed from 4 to 8 columns
                    try
                    {
                        Item item = backpack.getItemAtTile(i, j);
                        if (item.getItemID() == "BAC")
                        {
                            armorRow = i;
                            armorCol = j;
                            break;
                        }
                    }
                    catch (...)
                    {
                        // Skip exceptions
                    }
                }
                if (armorRow != -1)
                    break;
            }

            // Find empty slot
            findEmptySlot(backpack, emptyRow, emptyCol);

            if (armorRow != -1 && emptyRow != -1)
            {
                std::cout << "Moving Battle Armor from (" << armorRow << ","
                          << (char)('A' + armorCol) << ") to (" << emptyRow << ","
                          << (char)('A' + emptyCol) << ")..." << std::endl;
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

            // Test 2: Move different armor type to another slot
            // First, add a different armor type if needed
            int  breastplateRow = -1, breastplateCol = -1;
            bool hasBreastplate = findItemIdSlot(backpack, "BPA", breastplateRow, breastplateCol);

            if (!hasBreastplate)
            {
                // Add a breastplate to an empty slot
                if (findEmptySlot(backpack, breastplateRow, breastplateCol))
                {
                    std::cout << "Adding Breastplate to position (" << breastplateRow << ","
                              << (char)('A' + breastplateCol) << ")..." << std::endl;
                    backpack.addItemAtTile(
                        breastplateRow, breastplateCol, itemManager.getItem("BPA"), 1);

                    // Print updated backpack
                    std::cout << "=== Backpack After Adding Breastplate ===" << std::endl;
                    printBackpack(backpack);
                    std::cout << std::endl;
                }
            }

            // Find another empty slot
            int emptyRow2 = -1, emptyCol2 = -1;
            if (findEmptySlot(backpack, emptyRow2, emptyCol2) && (breastplateRow != -1))
            {
                std::cout << "Moving Breastplate from (" << breastplateRow << ","
                          << (char)('A' + breastplateCol) << ") to (" << emptyRow2 << ","
                          << (char)('A' + emptyCol2) << ")..." << std::endl;
                backpack.moveItem(breastplateRow, breastplateCol, emptyRow2, emptyCol2);

                // Print updated backpack
                std::cout << "=== Backpack After Moving Breastplate ===" << std::endl;
                printBackpack(backpack);
                std::cout << std::endl;
            }

            // Test 3: Move an item to a slot with a different item (swap)
            int  chainMailRow = -1, chainMailCol = -1;
            bool hasChainMail = findItemIdSlot(backpack, "CHN", chainMailRow, chainMailCol);

            if (!hasChainMail)
            {
                // Add chain mail to an empty slot
                if (findEmptySlot(backpack, chainMailRow, chainMailCol))
                {
                    std::cout << "Adding Chain Mail to position (" << chainMailRow << ","
                              << (char)('A' + chainMailCol) << ")..." << std::endl;
                    backpack.addItemAtTile(chainMailRow, chainMailCol, itemManager.getItem("CHN"), 1);

                    // Print updated backpack
                    std::cout << "=== Backpack After Adding Chain Mail ===" << std::endl;
                    printBackpack(backpack);
                    std::cout << std::endl;
                }
            }

            // Find a different item to swap with
            int  leatherRow = -1, leatherCol = -1;
            bool hasLeather = findItemIdSlot(backpack, "LTH", leatherRow, leatherCol);

            if (!hasLeather)
            {
                // Add leather armor to an empty slot
                if (findEmptySlot(backpack, leatherRow, leatherCol))
                {
                    std::cout << "Adding Leather Armor to position (" << leatherRow << ","
                              << (char)('A' + leatherCol) << ")..." << std::endl;
                    backpack.addItemAtTile(leatherRow, leatherCol, itemManager.getItem("LTH"), 1);

                    // Print updated backpack
                    std::cout << "=== Backpack After Adding Leather Armor ===" << std::endl;
                    printBackpack(backpack);
                    std::cout << std::endl;
                }
            }

            if (chainMailRow != -1 && leatherRow != -1)
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
            int sword1Row = -1, sword1Col = -1;
            int sword2Row = -1, sword2Col = -1;

            // Add two swords to empty slots
            findEmptySlot(backpack, sword1Row, sword1Col);
            if (sword1Row != -1)
            {
                backpack.addItemAtTile(sword1Row, sword1Col, itemManager.getItem("SWD"), 1);
                std::cout << "Added 1 sword to position (" << sword1Row << ","
                          << (char)('A' + sword1Col) << ")" << std::endl;
            }

            findEmptySlot(backpack, sword2Row, sword2Col);
            if (sword2Row != -1)
            {
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
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }

        // NEW: Test the filterItemsByType method
        std::cout << "=== Test Filtering Items by Type ===" << std::endl;
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
            std::cout << "Error: " << e.what() << std::endl;
        }

        // 13. Create and initialize equipment
        Equipment equipment;

        // 14. Print initial equipment state
        std::cout << "=== Initial Equipment State ===" << std::endl;
        printEquippedItems(equipment);
        std::cout << std::endl;

        // 15. Test equipping items from backpack
        std::cout << "=== Test Equipping Items from Backpack ===" << std::endl;
        try
        {
            // Add various equipment items to backpack for testing

            // Add boots to an empty slot
            int bootsRow = -1, bootsCol = -1;
            if (findEmptySlot(backpack, bootsRow, bootsCol))
            {
                std::cout << "Adding boots to position (" << bootsRow << ","
                          << (char)('A' + bootsCol) << ")..." << std::endl;
                backpack.addItemAtTile(bootsRow, bootsCol, itemManager.getItem("BOO"), 1);
            }
            else
            {
                std::cout << "No empty slot found for boots" << std::endl;
            }

            // Add sword to an empty slot
            int swordRow = -1, swordCol = -1;
            if (findEmptySlot(backpack, swordRow, swordCol))
            {
                std::cout << "Adding sword to position (" << swordRow << ","
                          << (char)('A' + swordCol) << ")..." << std::endl;
                backpack.addItemAtTile(swordRow, swordCol, itemManager.getItem("SWD"), 1);
            }
            else
            {
                std::cout << "No empty slot found for sword" << std::endl;
            }

            // Add helmet to an empty slot
            int helmetRow = -1, helmetCol = -1;
            if (findEmptySlot(backpack, helmetRow, helmetCol))
            {
                std::cout << "Adding helmet to position (" << helmetRow << ","
                          << (char)('A' + helmetCol) << ")..." << std::endl;
                backpack.addItemAtTile(helmetRow, helmetCol, itemManager.getItem("HLM"), 1);
            }
            else
            {
                std::cout << "No empty slot found for helmet" << std::endl;
            }

            // Add pendant to an empty slot
            int pendantRow = -1, pendantCol = -1;
            if (findEmptySlot(backpack, pendantRow, pendantCol))
            {
                std::cout << "Adding pendant to position (" << pendantRow << ","
                          << (char)('A' + pendantCol) << ")..." << std::endl;
                backpack.addItemAtTile(pendantRow, pendantCol, itemManager.getItem("NCK"), 1);
            }
            else
            {
                std::cout << "No empty slot found for pendant" << std::endl;
            }

            // Print the backpack before equipping
            std::cout << "=== Backpack Before Equipping ===" << std::endl;
            printBackpack(backpack);
            std::cout << std::endl;

            // Find and equip weapon
            int weaponRow = -1, weaponCol = -1;
            if (findItemTypeSlot(backpack, "Weapon", weaponRow, weaponCol))
            {
                std::cout << "Equipping weapon from position (" << weaponRow << ","
                          << (char)('A' + weaponCol) << ")..." << std::endl;
                bool weaponEquipped =
                    equipment.equipItemFromBackpack(backpack, weaponRow, weaponCol, "Weapon");
                std::cout << "Weapon " << (weaponEquipped ? "equipped" : "failed to equip")
                          << std::endl;
            }
            else
            {
                std::cout << "No weapon found in backpack" << std::endl;
            }

            // Find and equip head armor
            int headArmorRow = -1, headArmorCol = -1;
            if (findItemTypeSlot(backpack, "HeadArmor", headArmorRow, headArmorCol))
            {
                std::cout << "Equipping head armor from position (" << headArmorRow << ","
                          << (char)('A' + headArmorCol) << ")..." << std::endl;
                bool headEquipped = equipment.equipItemFromBackpack(
                    backpack, headArmorRow, headArmorCol, "HeadArmor");
                std::cout << "Head armor " << (headEquipped ? "equipped" : "failed to equip")
                          << std::endl;
            }
            else
            {
                std::cout << "No head armor found in backpack" << std::endl;
            }

            // Find and equip body armor
            int bodyArmorRow = -1, bodyArmorCol = -1;
            if (findItemTypeSlot(backpack, "BodyArmor", bodyArmorRow, bodyArmorCol))
            {
                std::cout << "Equipping body armor from position (" << bodyArmorRow << ","
                          << (char)('A' + bodyArmorCol) << ")..." << std::endl;
                bool bodyEquipped = equipment.equipItemFromBackpack(
                    backpack, bodyArmorRow, bodyArmorCol, "BodyArmor");
                std::cout << "Body armor " << (bodyEquipped ? "equipped" : "failed to equip")
                          << std::endl;
            }
            else
            {
                std::cout << "No body armor found in backpack" << std::endl;
            }

            // Find and equip foot armor
            int footArmorRow = -1, footArmorCol = -1;
            if (findItemTypeSlot(backpack, "FootArmor", footArmorRow, footArmorCol))
            {
                std::cout << "Equipping foot armor from position (" << footArmorRow << ","
                          << (char)('A' + footArmorCol) << ")..." << std::endl;
                bool footEquipped = equipment.equipItemFromBackpack(
                    backpack, footArmorRow, footArmorCol, "FootArmor");
                std::cout << "Foot armor " << (footEquipped ? "equipped" : "failed to equip")
                          << std::endl;
            }
            else
            {
                std::cout << "No foot armor found in backpack" << std::endl;
            }

            // Find and equip pendant
            int pendantEquipRow = -1, pendantEquipCol = -1;
            if (findItemTypeSlot(backpack, "Pendant", pendantEquipRow, pendantEquipCol))
            {
                std::cout << "Equipping pendant from position (" << pendantEquipRow << ","
                          << (char)('A' + pendantEquipCol) << ")..." << std::endl;
                bool pendantEquipped = equipment.equipItemFromBackpack(
                    backpack, pendantEquipRow, pendantEquipCol, "Pendant");
                std::cout << "Pendant " << (pendantEquipped ? "equipped" : "failed to equip")
                          << std::endl;
            }
            else
            {
                std::cout << "No pendant found in backpack" << std::endl;
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
            std::cout << "Error: " << e.what() << std::endl;
        }

        // 16. Test unequipping items to backpack
        std::cout << "=== Test Unequipping Items to Backpack ===" << std::endl;
        try
        {
            // *** Test 1: Unequip to empty slot ***
            // Find empty slot for the weapon
            int emptyWeaponRow = -1, emptyWeaponCol = -1;
            if (findEmptySlot(backpack, emptyWeaponRow, emptyWeaponCol))
            {
                std::cout << "Unequipping weapon to empty position (" << emptyWeaponRow << ","
                          << (char)('A' + emptyWeaponCol) << ")..." << std::endl;
                bool weaponUnequipped = equipment.unequipItemToBackpack(
                    backpack, emptyWeaponRow, emptyWeaponCol, "Weapon");
                std::cout << "Weapon " << (weaponUnequipped ? "unequipped" : "failed to unequip")
                          << std::endl;
            }
            else
            {
                std::cout << "No empty slot found for unequipping weapon" << std::endl;
            }

            printEquippedItems(equipment);
            printBackpack(backpack);

            // *** Test 2: Unequip to slot with same type (should swap) ***
            // Add another weapon to backpack
            int secondWeaponRow = -1, secondWeaponCol = -1;
            if (findEmptySlot(backpack, secondWeaponRow, secondWeaponCol))
            {
                std::cout << "Adding another weapon (Katana) to position (" << secondWeaponRow
                          << "," << (char)('A' + secondWeaponCol) << ")..." << std::endl;
                backpack.addItemAtTile(
                    secondWeaponRow, secondWeaponCol, itemManager.getItem("KAT"), 1);

                printBackpack(backpack);

                // Equip a different weapon first
                std::cout << "Equipping a sword as current weapon..." << std::endl;
                int swordRow = -1, swordCol = -1;
                if (findItemIdSlot(backpack, "SWD", swordRow, swordCol))
                {
                    equipment.equipItemFromBackpack(backpack, swordRow, swordCol, "Weapon");
                }
                else
                {
                    // If no sword found, add one to an empty slot and equip it
                    if (findEmptySlot(backpack, swordRow, swordCol))
                    {
                        backpack.addItemAtTile(swordRow, swordCol, itemManager.getItem("SWD"), 1);
                        equipment.equipItemFromBackpack(backpack, swordRow, swordCol, "Weapon");
                    }
                }

                // Print current equipment
                std::cout << "=== Current Equipment ===" << std::endl;

                printEquippedItems(equipment);
                printBackpack(backpack);

                std::cout << std::endl;

                // Now try to unequip to the slot with Katana (should swap)
                std::cout << "Unequipping weapon to position with same type (" << secondWeaponRow
                          << "," << (char)('A' + secondWeaponCol) << ")..." << std::endl;
                bool weaponSwapped = equipment.unequipItemToBackpack(
                    backpack, secondWeaponRow, secondWeaponCol, "Weapon");
                std::cout << "Weapon " << (weaponSwapped ? "swapped" : "failed to swap") << std::endl;
            }
            else
            {
                std::cout << "No empty slot found for testing weapon swap" << std::endl;
            }

            printEquippedItems(equipment);
            printBackpack(backpack);

            // *** Test 3: Unequip to slot with different type (should fail) ***
            // Find a slot with a potion
            int potionRow = -1, potionCol = -1;
            if (findItemTypeSlot(backpack, "Potion", potionRow, potionCol))
            {
                std::cout << "Trying to unequip body armor to slot with potion at (" << potionRow
                          << "," << (char)('A' + potionCol) << ")..." << std::endl;
                bool armorUnequipped =
                    equipment.unequipItemToBackpack(backpack, potionRow, potionCol, "BodyArmor");
                std::cout << "Body armor " << (armorUnequipped ? "unequipped" : "failed to unequip")
                          << " (should fail)" << std::endl;
            }
            else
            {
                // If no potion found, add one to an empty slot
                if (findEmptySlot(backpack, potionRow, potionCol))
                {
                    std::cout << "Adding a potion to test unequipping to different type..."
                              << std::endl;
                    backpack.addItemAtTile(potionRow, potionCol, itemManager.getItem("HPP"), 1);

                    std::cout << "Trying to unequip body armor to slot with potion at ("
                              << potionRow << "," << (char)('A' + potionCol) << ")..." << std::endl;
                    bool armorUnequipped =
                        equipment.unequipItemToBackpack(backpack, potionRow, potionCol, "BodyArmor");
                    std::cout << "Body armor "
                              << (armorUnequipped ? "unequipped" : "failed to unequip")
                              << " (should fail)" << std::endl;
                }
            }

            printEquippedItems(equipment);
            printBackpack(backpack);

            // *** Test 4: Unequip to slot with multiple items (stack) (should fail) ***
            // Find a slot with multiple items
            int stackRow = -1, stackCol = -1;
            for (int i = 0; i < 4; ++i)
            {  // MODIFIED: Changed from 8 to 4 rows
                for (int j = 0; j < 8; ++j)
                {  // MODIFIED: Changed from 4 to 8 columns
                    try
                    {
                        int quantity = backpack.getQuantityAtTile(i, j);
                        if (quantity > 1)
                        {
                            stackRow = i;
                            stackCol = j;
                            break;
                        }
                    }
                    catch (...)
                    {
                        // Skip exceptions
                    }
                }
                if (stackRow != -1)
                    break;
            }

            if (stackRow != -1)
            {
                std::cout << "Trying to unequip foot armor to slot with stack at (" << stackRow
                          << "," << (char)('A' + stackCol) << ")..." << std::endl;
                bool footUnequipped =
                    equipment.unequipItemToBackpack(backpack, stackRow, stackCol, "FootArmor");
                std::cout << "Foot armor " << (footUnequipped ? "unequipped" : "failed to unequip")
                          << " (should fail)" << std::endl;
            }
            else
            {
                std::cout << "No stack found for testing unequip to stack" << std::endl;
            }

            printEquippedItems(equipment);
            printBackpack(backpack);
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
        // 17. Test error handling
        std::cout << "=== Test Error Handling ===" << std::endl;

        // Test invalid coordinates
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
        try
        {
            std::cout << "Testing empty cell access at (0, 0)..." << std::endl;
            backpack.takeItemAtTile(0, 0, 1);
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // Test insufficient quantity
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
        try
        {
            std::cout << "Testing adding different item to occupied slot..." << std::endl;
            // Find a non-empty slot
            int occupiedRow = -1, occupiedCol = -1;
            for (int i = 0; i < 4 && occupiedRow == -1; i++)
            {  // MODIFIED: Changed from 8 to 4 rows
                for (int j = 0; j < 8 && occupiedRow == -1; j++)
                {  // MODIFIED: Changed from 4 to 8 columns
                    try
                    {
                        Item item = backpack.getItemAtTile(i, j);
                        // Found an occupied slot with a Weapon
                        if (item.getType() == "Weapon")
                        {
                            occupiedRow = i;
                            occupiedCol = j;
                            break;
                        }
                    }
                    catch (...)
                    {
                        // Skip empty cells
                    }
                }
            }

            if (occupiedRow != -1)
            {
                // Try to add a different item type to this slot
                std::cout << "Trying to add a Helmet to position (" << occupiedRow << ","
                          << (char)('A' + occupiedCol) << ") that has a Weapon..." << std::endl;
                backpack.addItemAtTile(occupiedRow, occupiedCol, itemManager.getItem("HLM"), 1);
            }
            else
            {
                std::cout << "Could not find an occupied slot with a Weapon for testing" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }

        // Test backpack full exception
        try
        {
            std::cout << "Testing backpack full exception..." << std::endl;

            // First, fill all remaining empty slots
            int filledCount = 0;
            for (int i = 0; i < 4; i++)
            {  // MODIFIED: Changed from 8 to 4 rows
                for (int j = 0; j < 8; j++)
                {  // MODIFIED: Changed from 4 to 8 columns
                    try
                    {
                        backpack.getItemAtTile(i, j);
                    }
                    catch (const EmptyCellException&)
                    {
                        // Found empty slot, fill it
                        try
                        {
                            backpack.addItemAtTile(i, j, itemManager.getItem("HPP"), 1);
                            filledCount++;
                        }
                        catch (...)
                        {
                            // Skip if adding fails
                        }
                    }
                    catch (...)
                    {
                        // Skip other exceptions
                    }
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

        std::cout << "=== All Tests Completed ===" << std::endl;

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