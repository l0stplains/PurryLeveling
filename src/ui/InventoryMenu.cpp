#include "ui/InventoryMenu.hpp"

#include <cstring>

InventoryMenu::DragDropPayload::DragDropPayload(bool fromEquip, int x, int y, const std::string& type)
    : fromEquipment(fromEquip), sourceX(x), sourceY(y)
{
    // copy at most 15 chars + null terminator
    std::strncpy(slotType, type.c_str(), sizeof(slotType) - 1);
    slotType[sizeof(slotType) - 1] = '\0';
}

InventoryMenu::InventoryMenu(GameContext& gameContext)
    : m_gameContext(gameContext),
      m_backpack(*gameContext.GetBackpack()),
      m_equipment(*gameContext.GetEquipment())
{
    std::vector<std::shared_ptr<Effect>> effects;
    m_draggedItem = std::make_unique<std::pair<Item, int>>(Item("", "", "", 'C', effects, ""), 0);
    m_isDraggingFromEquipment = false;
    m_equipmentSlotType       = "";

    // Initialize some sample items in m_backpack
    // Using proper Item constructor: Item(itemID, name, type, rarity, effects)
    m_backpack.addItem(Item("ice_potion", "Ice Potion", "Potion", 'A', effects, ""), 10);
    m_backpack.addItem(Item("diamond_ore", "Diamond", "resource", 'S', effects, ""), 5);
    m_backpack.addItem(Item("iron_ore", "Iron", "resource", 'C', effects, ""), 45);
    m_backpack.addItem(Item("gold_ore", "Gold", "resource", 'B', effects, ""), 12);
    m_backpack.addItem(Item("wood", "Wood", "resource", 'C', effects, ""), 32);
    m_backpack.addItem(Item("stone", "Stone", "resource", 'C', effects, ""), 64);

    // Add some m_equipment items
    m_backpack.addItem(Item("iron_sword", "Iron Sword", "Weapon", 'B', effects, ""), 5);
    m_backpack.addItem(Item("leather_helmet", "Leather Helmet", "HeadArmor", 'C', effects, ""), 1);
    m_backpack.addItem(Item("chain_mail", "Chain Mail", "BodyArmor", 'B', effects, ""), 1);
    m_backpack.addItem(Item("leather_boots", "Leather Boots", "FootArmor", 'C', effects, ""), 1);
    m_backpack.addItem(Item("magic_amulet", "Magic Amulet", "Pendant", 'A', effects, ""), 1);
}

void InventoryMenu::Render()
{
    m_isDragging = false;

    // Start the inventory window
    ImGui::Begin("Inventory",
                 nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

    // Set window size to fit both inventory and m_equipment
    ImGui::SetWindowSize(
        ImVec2(GRID_WIDTH * SLOT_SIZE + SLOT_PADDING * 2 + SLOT_SIZE * 2,
               GRID_HEIGHT * SLOT_SIZE + SLOT_PADDING * 2 + EQUIP_SLOTS * SLOT_SIZE + 40));

    // Calculate positions for grid
    ImVec2 curPos = {125.f, 150.f};
    float  startX = curPos.x + SLOT_PADDING;
    float  startY = curPos.y + SLOT_PADDING;

    // Render m_backpack grid
    RenderBackpack(startX, startY);

    // Render m_equipment slots (to the right of m_backpack)
    float equipmentStartX = startX + GRID_WIDTH * SLOT_SIZE + SLOT_PADDING;
    RenderEquipment(equipmentStartX, startY);

    // Add some control buttons
    ImGui::SetCursorScreenPos(ImVec2(startX, startY + GRID_HEIGHT * SLOT_SIZE + 10));
    /**
    if (ImGui::Button("Add Random Items"))
    {
        AddRandomItems();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear All"))
    {
        ClearInventory();
    }
        */

    ImGui::End();
}

void InventoryMenu::RenderBackpack(float startX, float startY)
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
                // Handle single-click on m_backpack slot
            }

            // Render item in m_backpack slot if it exists
            try
            {
                Item item  = m_backpack.getItemAtTile(y, x);
                int  count = m_backpack.getQuantityAtTile(y, x);

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    std::string itemType = item.getType();
                    if (itemType == "Weapon" || itemType == "HeadArmor" ||
                        itemType == "BodyArmor" || itemType == "FootArmor" || itemType == "Pendant")
                    {
                        m_equipment.equipItemFromBackpack(m_backpack, y, x, itemType);
                        // TODO: handle full inventory
                    }
                }
                // Drag source - create for non-empty slots
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    m_isDragging = true;
                    // Use consistent coordinates - store m_backpack coordinates directly
                    m_dragSourceX             = x;
                    m_dragSourceY             = y;
                    m_isDraggingFromEquipment = false;
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

                sf::Texture* texture =
                    &m_gameContext.GetResourceManager()->GetTexture("ice_potion");  // Get texture
                                                                                    // by item ID
                                                                                    // (TODO)
                if (texture)
                {
                    ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();
                    ImGui::Image(textureId, ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
                }
                else
                {
                    // Current implementation using colored buttons
                    ImGui::PushStyleColor(ImGuiCol_Button,
                                          GetItemColor(item.getName(), item.getType()));

                    ImGui::Button(item.getName().c_str(), ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
                    ImGui::PopStyleColor();
                }
                /**
                // Current implementation using colored buttons
                ImGui::PushStyleColor(ImGuiCol_Button, GetItemColor(item.getName(),
                item.getType()));

                ImGui::Button(item.getName().c_str(), ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
                ImGui::PopStyleColor();

                */

                // Show stack count for items with count > 1
                if (count > 1)
                {
                    std::string countText = std::to_string(count);
                    ImVec2      textSize  = ImGui::CalcTextSize(countText.c_str());
                    ImVec2      textPos   = ImVec2(startX + (x + 1) * SLOT_SIZE - textSize.x - 15,
                                            startY + (y + 1) * SLOT_SIZE - textSize.y - 15);

                    // draw it directly—does NOT create a new ImGui item!
                    ImGui::GetWindowDrawList()->AddText(textPos,
                                                        IM_COL32(255, 255, 255, 255),  // white,
                                                                                       // full
                                                                                       // opacity
                                                        countText.c_str());
                }
                char        r = item.getRarity();
                std::string rarityText(1, r);
                ImVec2      rarityTextSize = ImGui::CalcTextSize(rarityText.c_str());
                ImVec2 textPos = ImVec2(startX + x * SLOT_SIZE + 7, startY + y * SLOT_SIZE + 5);

                ImGui::GetWindowDrawList()->AddText(textPos,
                                                    IM_COL32(255, 255, 255, 255),  // white,
                                                                                   // full
                                                                                   // opacity
                                                    rarityText.c_str());
            }
            catch (const std::exception& e)
            {
                // No item in this slot
            }

            // Drop target - for m_backpack slots
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
                        m_backpack.getItemAtTile(y, x);
                    }
                    catch (const std::exception&)
                    {
                        slotEmpty = true;
                    }

                    // 2) Handle equip→m_backpack first:
                    if (payload->fromEquipment)
                    {
                        m_equipment.unequipItemToBackpack(m_backpack, y, x, payload->slotType);
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
                                if (m_backpack.getQuantityAtTile(payload->sourceY,
                                                                 payload->sourceX) > 1)
                                {
                                    Item one = m_backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, 1);
                                    m_backpack.addItemAtTile(y, x, one, 1);
                                }
                            }
                            else if (ctrl)
                            {
                                // Ctrl: take half
                                int qty =
                                    m_backpack.getQuantityAtTile(payload->sourceY, payload->sourceX);
                                if (qty > 1)
                                {
                                    int  half     = qty / 2;
                                    Item halfItem = m_backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, half);
                                    m_backpack.addItemAtTile(y, x, halfItem, half);
                                }
                            }
                            else
                            {
                                // No modifier: move entire stack
                                m_backpack.moveItem(payload->sourceY, payload->sourceX, y, x);
                            }
                        }
                        // 3b) NON-EMPTY destination
                        else
                        {
                            Item dest = m_backpack.getItemAtTile(y, x);
                            Item src = m_backpack.getItemAtTile(payload->sourceY, payload->sourceX);

                            // 3b–i) Same item → stack them
                            if (dest.getName() == src.getName())
                            {
                                int srcQty =
                                    m_backpack.getQuantityAtTile(payload->sourceY, payload->sourceX);
                                int dstQty   = m_backpack.getQuantityAtTile(y, x);
                                int transfer = std::min(srcQty, MAX_STACK_SIZE - dstQty);
                                if (transfer > 0)
                                {
                                    m_backpack.addItemAtTile(y, x, dest, transfer);
                                    m_backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, transfer);
                                }
                            }
                            // 3b–ii) Different item → respect shift/ctrl/move
                            else if (shift)
                            {
                                // Shift: take exactly one
                                if (m_backpack.getQuantityAtTile(payload->sourceY,
                                                                 payload->sourceX) > 1)
                                {
                                    Item one = m_backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, 1);
                                    m_backpack.addItemAtTile(y, x, one, 1);
                                }
                            }
                            else if (ctrl)
                            {
                                // Ctrl: take half
                                int qty =
                                    m_backpack.getQuantityAtTile(payload->sourceY, payload->sourceX);
                                if (qty > 1)
                                {
                                    int  half     = qty / 2;
                                    Item halfItem = m_backpack.takeItemAtTile(
                                        payload->sourceY, payload->sourceX, half);
                                    m_backpack.addItemAtTile(y, x, halfItem, half);
                                }
                            }
                            else
                            {
                                // No modifier: full move swaps positions
                                m_backpack.moveItem(payload->sourceY, payload->sourceX, y, x);
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

void InventoryMenu::RenderEquipment(float startX, float startY)
{
    // m_equipment slot names for display
    const std::string slotNames[EQUIP_SLOTS] = {
        "Weapon", "Head Armor", "Body Armor", "Foot Armor", "Pendant"};

    // m_equipment type identifiers
    const std::string slotTypes[EQUIP_SLOTS] = {
        "Weapon", "HeadArmor", "BodyArmor", "FootArmor", "Pendant"};

    for (int i = 0; i < EQUIP_SLOTS; i++)
    {
        ImGui::SetCursorScreenPos(
            ImVec2(startX + SLOT_SIZE + 8 + 150, startY + i * SLOT_SIZE + SLOT_SIZE / 2.0f - 10));

        // Create a label for the m_equipment slot
        ImGui::Text("%s", slotNames[i].c_str());

        // Position the actual slot
        ImGui::SetCursorScreenPos(ImVec2(startX + 200, startY + i * SLOT_SIZE));

        // Create a selectable area for the m_equipment slot
        ImGui::PushID(1000 + i);  // Use a different ID range than m_backpack slots
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.6f, 1.0f));

        if (ImGui::Button("", ImVec2(SLOT_SIZE - 8, SLOT_SIZE - 8)))
        {
            // Single-click handling for m_equipment slots
            std::cout << "Clicked on " << slotNames[i] << " slot" << std::endl;
        }

        // Get the item in this m_equipment slot
        Item equippedItem;
        switch (i)
        {
            case 0:
                equippedItem = m_equipment.getWeapon();
                break;
            case 1:
                equippedItem = m_equipment.getHeadArmor();
                break;
            case 2:
                equippedItem = m_equipment.getBodyArmor();
                break;
            case 3:
                equippedItem = m_equipment.getFootArmor();
                break;
            case 4:
                equippedItem = m_equipment.getPendant();
                break;
        }

        // If there's an item equipped, render it and set up drag source
        if (!equippedItem.isNull())
        {
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                // 0 0 so it forces the function to find the best position
                m_equipment.unequipItemToBackpack(m_backpack, 0, 0, slotTypes[i]);
            }
            // Create drag source for m_equipment
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                m_isDragging              = true;
                m_isDraggingFromEquipment = true;
                m_equipmentSlotType       = slotTypes[i];

                // Prepare the payload with our POD-safe class
                DragDropPayload payload(/*fromm_equipment=*/true,
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
            // OPTION: To use images for m_equipment items
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
                                  GetItemColor(equippedItem.getName(), equippedItem.getType()));
            ImGui::Button(equippedItem.getName().c_str(), ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
            ImGui::PopStyleColor();

            char        r = equippedItem.getRarity();
            std::string rarityText(1, r);
            ImVec2      rarityTextSize = ImGui::CalcTextSize(rarityText.c_str());
            ImVec2      textPos        = ImVec2(startX * SLOT_SIZE + 7, startY + (i)*SLOT_SIZE + 5);

            ImGui::GetWindowDrawList()->AddText(textPos,
                                                IM_COL32(255, 255, 255, 255),  // white,
                                                                               // full
                                                                               // opacity
                                                rarityText.c_str());
        }

        // Create drop target for m_equipment slots
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
                    try
                    {
                        m_equipment.equipItemFromBackpack(
                            m_backpack, payload->sourceY, payload->sourceX, slotTypes[i]);
                    }
                    catch (InvalidEquipmentTypeException e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
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
ImVec4 InventoryMenu::GetItemColor(const std::string& itemName, const std::string& itemType)
{
    // m_equipment type colors
    if (itemType == "Weapon")
        return ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    if (itemType == "HeadArmor" || itemType == "BodyArmor" || itemType == "FootArmor")
        return ImVec4(0.2f, 0.6f, 0.8f, 1.0f);
    if (itemType == "Pendant")
        return ImVec4(0.8f, 0.6f, 0.2f, 1.0f);

    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}
