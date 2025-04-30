#include "ui/InventorySystem.hpp"

#include <cstring>

InventorySystem::DragDropPayload::DragDropPayload(bool fromEquip, int x, int y, const std::string& type)
    : fromEquipment(fromEquip), sourceX(x), sourceY(y)
{
    // copy at most 15 chars + null terminator
    std::strncpy(slotType, type.c_str(), sizeof(slotType) - 1);
    slotType[sizeof(slotType) - 1] = '\0';
}

InventorySystem::InventorySystem()
    : backpack(std::vector<std::pair<Item, int>>(), 4, 8), equipment()
{
    draggedItem             = std::make_unique<std::pair<Item, int>>(Item("", "", "", 'C', {}), 0);
    isDraggingFromEquipment = false;
    equipmentSlotType       = "";

    // Initialize some sample items in backpack
    // Using proper Item constructor: Item(itemID, name, type, rarity, effects)
    backpack.addItem(Item("diamond_ore", "Diamond", "resource", 'S', {}), 5);
    backpack.addItem(Item("iron_ore", "Iron", "resource", 'C', {}), 45);
    backpack.addItem(Item("gold_ore", "Gold", "resource", 'B', {}), 12);
    backpack.addItem(Item("wood", "Wood", "resource", 'C', {}), 32);
    backpack.addItem(Item("stone", "Stone", "resource", 'C', {}), 64);

    // Add some equipment items
    backpack.addItem(Item("iron_sword", "Iron Sword", "Weapon", 'B', {"dmg+5"}), 5);
    backpack.addItem(Item("leather_helmet", "Leather Helmet", "HeadArmor", 'C', {"def+2"}), 1);
    backpack.addItem(Item("chain_mail", "Chain Mail", "BodyArmor", 'B', {"def+5"}), 1);
    backpack.addItem(Item("leather_boots", "Leather Boots", "FootArmor", 'C', {"spd+1"}), 1);
    backpack.addItem(Item("magic_amulet", "Magic Amulet", "Pendant", 'A', {"mp+10"}), 1);

    // Set initial equipment
}

void InventorySystem::render()
{
    isDragging = false;

    // Start the inventory window
    ImGui::Begin(
        "Inventory", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);

    // Set window size to fit both inventory and equipment
    ImGui::SetWindowSize(
        ImVec2(GRID_WIDTH * SLOT_SIZE + SLOT_PADDING * 2 + SLOT_SIZE * 2,
               GRID_HEIGHT * SLOT_SIZE + SLOT_PADDING * 2 + EQUIP_SLOTS * SLOT_SIZE + 40));

    // Calculate positions for grid
    ImVec2 curPos = ImGui::GetCursorScreenPos();
    float  startX = curPos.x + SLOT_PADDING;
    float  startY = curPos.y + SLOT_PADDING;

    // Render backpack grid
    renderBackpack(startX, startY);

    // Render equipment slots (to the right of backpack)
    float equipmentStartX = startX + GRID_WIDTH * SLOT_SIZE + SLOT_PADDING;
    renderEquipment(equipmentStartX, startY);

    // Add some control buttons
    ImGui::SetCursorScreenPos(ImVec2(startX, startY + GRID_HEIGHT * SLOT_SIZE + 10));
    if (ImGui::Button("Add Random Items"))
    {
        addRandomItems();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear All"))
    {
        clearInventory();
    }

    ImGui::End();
}

void InventorySystem::renderBackpack(float startX, float startY)
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            // Set position for this slot
            ImGui::SetCursorScreenPos(ImVec2(startX + x * SLOT_SIZE, startY + y * SLOT_SIZE));

            // Create a selectable area for the slot
            ImGui::PushID(y * GRID_WIDTH + x);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

            if (ImGui::Button("", ImVec2(SLOT_SIZE - 8, SLOT_SIZE - 8)))
            {
                // Handle single-click on backpack slot
            }

            // Render item in backpack slot if it exists
            try
            {
                Item item  = backpack.getItemAtTile(y, x);
                int  count = backpack.getQuantityAtTile(y, x);

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    std::string itemType = item.getType();
                    if (itemType == "Weapon" || itemType == "HeadArmor" ||
                        itemType == "BodyArmor" || itemType == "FootArmor" || itemType == "Pendant")
                    {
                        equipment.equipItemFromBackpack(backpack, y, x, itemType);
                        // TODO: handle full inventory
                    }
                }
                // Drag source - create for non-empty slots
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    isDragging = true;
                    // Use consistent coordinates - store backpack coordinates directly
                    dragSourceX             = x;
                    dragSourceY             = y;
                    isDraggingFromEquipment = false;
                    // Prepare the payload with our POD-safe class
                    DragDropPayload payload(false, x, y, "");
                    ImGui::SetDragDropPayload("INVENTORY_ITEM", &payload, sizeof(payload));
                    // Show item being dragged
                    ImGui::Text("%s x%d", item.getName().c_str(), count);

                    ImGui::EndDragDropSource();
                }

                // Render the item
                ImGui::SetCursorScreenPos(
                    ImVec2(startX + x * SLOT_SIZE + 4, startY + y * SLOT_SIZE + 4));

                /*
                // OPTION 1: To use images as background for items, you would replace the colored
                button with: if (item.texture.isValid()) { // Assuming you have a texture in your
                Item class
                    // Convert sf::Texture to ImGui texture
                    ImTextureID textureId = (ImTextureID)(intptr_t)item.texture.getNativeHandle();
                    ImGui::Image(textureId, ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
                } else {
                    // Fallback if no texture is available
                    ImGui::PushStyleColor(ImGuiCol_Button, getItemColor(item.getName(),
                item.getType())); ImGui::Button(item.getName().c_str(), ImVec2(SLOT_SIZE - 16,
                SLOT_SIZE - 16)); ImGui::PopStyleColor();
                }
                */

                /*
                // OPTION 2: Alternative approach with a texture manager
                sf::Texture* texture = textureManager.getTexture(item.getID()); // Get texture by
                item ID if (texture) { ImTextureID textureId =
                (ImTextureID)(intptr_t)texture->getNativeHandle(); ImGui::Image(textureId,
                ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16)); } else {
                    // Fallback coloring
                    ImGui::PushStyleColor(ImGuiCol_Button, getItemColor(item.getName(),
                item.getType())); ImGui::Button(item.getName().c_str(), ImVec2(SLOT_SIZE - 16,
                SLOT_SIZE - 16)); ImGui::PopStyleColor();
                }
                */

                // Current implementation using colored buttons
                ImGui::PushStyleColor(ImGuiCol_Button, getItemColor(item.getName(), item.getType()));

                ImGui::Button(item.getName().c_str(), ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
                ImGui::PopStyleColor();

                // Show stack count for items with count > 1
                if (count > 1)
                {
                    std::string countText = std::to_string(count);
                    ImVec2      textSize  = ImGui::CalcTextSize(countText.c_str());
                    ImVec2      textPos   = ImVec2(startX + (x + 1) * SLOT_SIZE - textSize.x - 10,
                                            startY + (y + 1) * SLOT_SIZE - textSize.y - 8);

                    // draw it directly—does NOT create a new ImGui item!
                    ImGui::GetWindowDrawList()->AddText(textPos,
                                                        IM_COL32(255, 255, 255, 255),  // white,
                                                                                       // full
                                                                                       // opacity
                                                        countText.c_str());
                }
            }
            catch (const std::exception& e)
            {
                // No item in this slot
            }

            // Drop target - for backpack slots
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("INVENTORY_ITEM"))
                {
                    const auto* payload = (const DragDropPayload*)p->Data;
                    auto&       io      = ImGui::GetIO();
                    bool        shift   = (io.KeyMods & ImGuiKey_ModShift) != 0;
                    bool        ctrl    = (io.KeyMods & ImGuiKey_ModCtrl) != 0;

                    // 1) Detect empty slot without swallowing all logic in a try/catch:
                    bool slotEmpty = false;
                    try
                    {
                        backpack.getItemAtTile(y, x);
                    }
                    catch (const std::exception&)
                    {
                        slotEmpty = true;
                    }

                    // 2) Handle equip→backpack first:
                    if (payload->fromEquipment)
                    {
                        equipment.unequipItemToBackpack(backpack, y, x, payload->slotType);
                        // TODO: handle full inventory
                    }
                    else
                    {
                        // 3a) EMPTY destination
                        if (slotEmpty)
                        {
                            if (shift)
                            {
                                // Shift: take exactly one
                                if (backpack.getQuantityAtTile(payload->sourceY, payload->sourceX) > 1)
                                {
                                    Item one = backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, 1);
                                    backpack.addItemAtTile(y, x, one, 1);
                                }
                            }
                            else if (ctrl)
                            {
                                // Ctrl: take half
                                int qty =
                                    backpack.getQuantityAtTile(payload->sourceY, payload->sourceX);
                                if (qty > 1)
                                {
                                    int  half     = qty / 2;
                                    Item halfItem = backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, half);
                                    backpack.addItemAtTile(y, x, halfItem, half);
                                }
                            }
                            else
                            {
                                // No modifier: move entire stack
                                backpack.moveItem(payload->sourceY, payload->sourceX, y, x);
                            }
                        }
                        // 3b) NON-EMPTY destination
                        else
                        {
                            Item dest = backpack.getItemAtTile(y, x);
                            Item src  = backpack.getItemAtTile(payload->sourceY, payload->sourceX);

                            // 3b–i) Same item → stack them
                            if (dest.getName() == src.getName())
                            {
                                int srcQty =
                                    backpack.getQuantityAtTile(payload->sourceY, payload->sourceX);
                                int dstQty   = backpack.getQuantityAtTile(y, x);
                                int transfer = std::min(srcQty, MAX_STACK_SIZE - dstQty);
                                if (transfer > 0)
                                {
                                    backpack.addItemAtTile(y, x, dest, transfer);
                                    backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, transfer);
                                }
                            }
                            // 3b–ii) Different item → respect shift/ctrl/move
                            else if (shift)
                            {
                                // Shift: take exactly one
                                if (backpack.getQuantityAtTile(payload->sourceY, payload->sourceX) > 1)
                                {
                                    Item one = backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, 1);
                                    backpack.addItemAtTile(y, x, one, 1);
                                }
                            }
                            else if (ctrl)
                            {
                                // Ctrl: take half
                                int qty =
                                    backpack.getQuantityAtTile(payload->sourceY, payload->sourceX);
                                if (qty > 1)
                                {
                                    int  half     = qty / 2;
                                    Item halfItem = backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, half);
                                    backpack.addItemAtTile(y, x, halfItem, half);
                                }
                            }
                            else
                            {
                                // No modifier: full move swaps positions
                                backpack.moveItem(payload->sourceY, payload->sourceX, y, x);
                            }
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
    }
}

void InventorySystem::renderEquipment(float startX, float startY)
{
    // Equipment slot names for display
    const std::string slotNames[EQUIP_SLOTS] = {
        "Weapon", "Head Armor", "Body Armor", "Foot Armor", "Pendant"};

    // Equipment type identifiers
    const std::string slotTypes[EQUIP_SLOTS] = {
        "Weapon", "HeadArmor", "BodyArmor", "FootArmor", "Pendant"};

    for (int i = 0; i < EQUIP_SLOTS; i++)
    {
        // Position each equipment slot
        ImGui::SetCursorScreenPos(
            ImVec2(startX + SLOT_SIZE + 8, startY + i * SLOT_SIZE + SLOT_SIZE / 2.0f - 5));

        // Create a label for the equipment slot
        ImGui::Text("%s", slotNames[i].c_str());

        // Position the actual slot
        ImGui::SetCursorScreenPos(ImVec2(startX, startY + i * SLOT_SIZE));

        // Create a selectable area for the equipment slot
        ImGui::PushID(1000 + i);  // Use a different ID range than backpack slots
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.6f, 1.0f));

        if (ImGui::Button("", ImVec2(SLOT_SIZE - 8, SLOT_SIZE - 8)))
        {
            // Single-click handling for equipment slots
            std::cout << "Clicked on " << slotNames[i] << " slot" << std::endl;
        }

        // Get the item in this equipment slot
        Item equippedItem;
        switch (i)
        {
            case 0:
                equippedItem = equipment.getWeapon();
                break;
            case 1:
                equippedItem = equipment.getHeadArmor();
                break;
            case 2:
                equippedItem = equipment.getBodyArmor();
                break;
            case 3:
                equippedItem = equipment.getFootArmor();
                break;
            case 4:
                equippedItem = equipment.getPendant();
                break;
        }

        // If there's an item equipped, render it and set up drag source
        if (!equippedItem.isNull())
        {
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                // 0 0 so it forces the function to find the best position
                equipment.unequipItemToBackpack(backpack, 0, 0, slotTypes[i]);
            }
            // Create drag source for equipment
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                isDragging              = true;
                isDraggingFromEquipment = true;
                equipmentSlotType       = slotTypes[i];

                // Prepare the payload with our POD-safe class
                DragDropPayload payload(/*fromEquipment=*/true,
                                        /*sourceX=*/-1,
                                        /*sourceY=*/-1,
                                        /*slotType*/ slotTypes[i]);
                ImGui::SetDragDropPayload("INVENTORY_ITEM", &payload, sizeof(payload));

                // Show item being dragged
                ImGui::Text("%s", equippedItem.getName().c_str());

                ImGui::EndDragDropSource();
            }
            // Render the equipped item
            ImGui::SetCursorScreenPos(ImVec2(startX + 4, startY + i * SLOT_SIZE + 4));

            /*
            // OPTION: To use images for equipment items
            sf::Texture* texture = textureManager.getTexture(equippedItem.getID());
            if (texture) {
                ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();
                ImGui::Image(textureId, ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
            } else {
                // Fallback to colored button
                ImGui::PushStyleColor(ImGuiCol_Button, getItemColor(equippedItem.getName(),
            equippedItem.getType())); ImGui::Button(equippedItem.getName().c_str(),
            ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16)); ImGui::PopStyleColor();
            }
            */

            // Current implementation using colored buttons
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  getItemColor(equippedItem.getName(), equippedItem.getType()));
            ImGui::Button(equippedItem.getName().c_str(), ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
            ImGui::PopStyleColor();
        }

        // Create drop target for equipment slots
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* imgui_payload = ImGui::AcceptDragDropPayload("INVENTORY_"
                                                                                 "ITEM"))
            {
                const DragDropPayload* payload = (const DragDropPayload*)imgui_payload->Data;

                if (!payload->fromEquipment)
                {
                    // Try to equip from backpack to this slot
                    // Fix: Pass the correct coordinates to equipItemFromBackpack
                    equipment.equipItemFromBackpack(
                        backpack, payload->sourceY, payload->sourceX, slotTypes[i]);
                }
                else
                {
                    // Equipment to equipment swap logic could be added here if needed
                    std::cout << "Equipment to equipment transfer not implemented" << std::endl;
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}

// Helper function to get color for item visualization
ImVec4 InventorySystem::getItemColor(const std::string& itemName, const std::string& itemType)
{
    // Equipment type colors
    if (itemType == "Weapon")
        return ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    if (itemType == "HeadArmor" || itemType == "BodyArmor" || itemType == "FootArmor")
        return ImVec4(0.2f, 0.6f, 0.8f, 1.0f);
    if (itemType == "Pendant")
        return ImVec4(0.8f, 0.6f, 0.2f, 1.0f);

    // Resource colors
    if (itemName == "Diamond")
        return ImVec4(0.0f, 0.8f, 1.0f, 1.0f);
    if (itemName == "Iron")
        return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    if (itemName == "Gold")
        return ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
    if (itemName == "Stone")
        return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    if (itemName == "Wood")
        return ImVec4(0.6f, 0.4f, 0.2f, 1.0f);

    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

// Add some random items to inventory for testing
void InventorySystem::addRandomItems()
{
    const std::vector<std::pair<std::string, std::string>> itemTypes = {
        {"Diamond", "resource"},
        {"Iron", "resource"},
        {"Gold", "resource"},
        {"Stone", "resource"},
        {"Wood", "resource"},
        {"Iron Sword", "Weapon"},
        {"Leather Helmet", "HeadArmor"},
        {"Chain Mail", "BodyArmor"},
        {"Leather Boots", "FootArmor"},
        {"Magic Amulet", "Pendant"}};

    const std::vector<char> rarities = {'D', 'C', 'B', 'A', 'S'};

    for (int i = 0; i < 5; i++)  // Add 5 random items
    {
        int randomIndex            = rand() % itemTypes.size();
        auto& [itemName, itemType] = itemTypes[randomIndex];
        int  itemCount             = (itemType == "resource") ? (1 + rand() % 64) : 1;
        char rarity                = rarities[rand() % rarities.size()];

        std::vector<std::string> effects;
        if (itemType != "resource")
        {
            effects.push_back("stat+" + std::to_string(1 + rand() % 10));
        }

        try
        {
            std::string itemID = itemName;
            std::transform(itemID.begin(), itemID.end(), itemID.begin(), ::tolower);
            backpack.addItem(Item(itemID, itemName, itemType, rarity, effects), itemCount);
        }
        catch (const std::exception& e)
        {
            // Backpack might be full
            std::cout << "Failed to add item: " << e.what() << std::endl;
        }
    }
}

// Clear all inventory slots
void InventorySystem::clearInventory()
{
    // Clear backpack
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            try
            {
                backpack.takeItemAtTile(y, x, backpack.getQuantityAtTile(y, x));
            }
            catch (const std::exception& e)
            {
                // No item in this slot
            }
        }
    }

    // Clear equipment slots
    equipment.setWeapon(Item());
    equipment.setHeadArmor(Item());
    equipment.setBodyArmor(Item());
    equipment.setFootArmor(Item());
    equipment.setPendant(Item());
}