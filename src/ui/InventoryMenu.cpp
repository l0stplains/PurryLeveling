#include "ui/InventoryMenu.hpp"

#include <cstring>

static constexpr int BACKPACK_SLOT_SIZE  = 112;
static constexpr int EQUIPMENT_SLOT_SIZE = 100;
static constexpr int SLOT_SIZE           = 92;
static constexpr int SLOT_PADDING        = 12;
static constexpr int MAX_STACK_SIZE      = 64;
static constexpr int EQUIP_SLOTS         = 5;
static constexpr int GRID_WIDTH          = 8;
static constexpr int GRID_HEIGHT         = 4;

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
      m_equipment(*gameContext.GetEquipment()),
      m_showErrorPopup(false)  // Initialize to false
{
    std::vector<std::string> effects;
    m_draggedItem = std::make_unique<std::pair<Item, int>>(Item("", "", "", 'C', effects, ""), 0);
    m_isDraggingFromEquipment = false;
    m_equipmentSlotType       = "";
}

void InventoryMenu::Render()
{
    m_isDragging = false;

    // clear last frame's description
    m_hoveredDescription.clear();

    // Start the inventory window
    // —— shop-style: no window/item padding ——
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::Begin("Inventory",
                 nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

    // Set window size to fit both inventory and m_equipment
    ImGui::SetWindowSize(
        ImVec2(GRID_WIDTH * EQUIPMENT_SLOT_SIZE + SLOT_PADDING * 2 + EQUIPMENT_SLOT_SIZE * 2,
               GRID_HEIGHT * EQUIPMENT_SLOT_SIZE + SLOT_PADDING * 2 +
                   EQUIP_SLOTS * EQUIPMENT_SLOT_SIZE + 40));

    // Calculate positions for grid - Adjusted to start higher
    ImVec2 curPos = {110.f, 130.f};
    float  startX = curPos.x + SLOT_PADDING;
    float  startY = curPos.y + SLOT_PADDING;

    // Render m_backpack grid
    RenderBackpack(startX, startY);

    // Render m_equipment slots (to the right of m_backpack)
    // Move equipment slots further to the right
    float equipmentStartX = startX + GRID_WIDTH * EQUIPMENT_SLOT_SIZE + SLOT_PADDING + 150;
    RenderEquipment(equipmentStartX, startY);

    // Render description box
    RenderDescriptionBox(startX, startY);

    // Render any error popups
    RenderErrorPopups();

    ImGui::PopStyleVar(4);  // pop WindowPadding, ItemSpacing, ItemInnerSpacing, FramePadding
    ImGui::End();
}

void InventoryMenu::RenderDescriptionBox(float startX, float startY)
{
    // Description box with brownish theme
    ImGui::SetCursorScreenPos(ImVec2(startX - 32, startY + GRID_HEIGHT * (BACKPACK_SLOT_SIZE - 2)));

    float descW = GRID_WIDTH * BACKPACK_SLOT_SIZE - 11;
    float descH = 52.0f;

    // Set up brown box style for description
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.25f, 0.15f, 0.1f, 0.9f));  // Dark brown
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));  // Darker brown
                                                                                // border

    // begin child w/ border
    ImGui::SetCursorScreenPos(ImVec2(startX - 32, startY + GRID_HEIGHT * (BACKPACK_SLOT_SIZE - 2)));
    ImGui::BeginChild("ItemDescription",
                      ImVec2(descW, descH),
                      /*border=*/true);

    // pick up whatever description or fallback
    std::string txt = m_hoveredDescription.empty() ? "Hover over an item to see its description."
                                                   : m_hoveredDescription;

    // Get window size for proper vertical centering
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize   = ImGui::CalcTextSize(txt.c_str());

    // Calculate position to center both horizontally and vertically
    float posX = (windowSize.x - textSize.x) * 0.5f;
    float posY = (windowSize.y - textSize.y) * 0.5f;

    // Set cursor position for perfect centering
    ImGui::SetCursorPos(ImVec2(posX, posY));

    // draw the text
    ImGui::TextUnformatted(txt.c_str());

    ImGui::EndChild();

    // Pop description box styles
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}

void InventoryMenu::RenderErrorPopups()
{
    // Check if we need to show any error popup
    if (!m_showErrorPopup)
    {
        return;
    }

    // Determine the popup title based on the error message
    std::string popupTitle = "Error";

    if (m_errorMessage.find("backpack is full") != std::string::npos)
    {
        popupTitle = "Backpack Full";
    }
    else if (m_errorMessage.find("Cannot equip") != std::string::npos)
    {
        popupTitle = "Wrong Item Type";
    }
    else if (m_errorMessage.find("Equipment to equipment") != std::string::npos)
    {
        popupTitle = "Unsupported Operation";
    }

    // Open the popup with the appropriate title
    ImGui::OpenPopup(popupTitle.c_str());

    // Center the popup
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    // Apply consistent styling for all error popups
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));

    // Handle all possible error popup types
    if (ImGui::BeginPopupModal(popupTitle.c_str(),
                               NULL,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s", m_errorMessage.c_str());  // Error
                                                                                           // message
                                                                                           // in red

        // Add spacing between error message and button
        ImGui::Dummy(ImVec2(0, 20));  // Add 20 pixels of vertical spacing

        float windowWidth  = ImGui::GetWindowSize().x;
        float buttonWidth  = 120.0f;
        float buttonStartX = (windowWidth - buttonWidth) * 0.5f;

        // Set cursor position to center the button
        ImGui::SetCursorPosX(buttonStartX);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

        if (ImGui::Button("OK", ImVec2(buttonWidth, 30)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        ImGui::EndPopup();
    }

    // Reset the flag after attempting to show the popup
    // Only reset if the popup is not showing (this is a critical fix)
    if (!ImGui::IsPopupOpen(popupTitle.c_str()))
    {
        m_showErrorPopup = false;
    }

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);
}

void InventoryMenu::RenderBackpack(float startX, float startY)
{
    // Calculate precise window dimensions for backpack
    float windowWidth  = GRID_WIDTH * (BACKPACK_SLOT_SIZE - 2) + 2;
    float windowHeight = GRID_HEIGHT * (BACKPACK_SLOT_SIZE - 2) + 2;

    // Position the window
    ImVec2 windowPos = ImVec2(startX - 31, startY);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Apply brownish style similar to ShopMenu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    // Set colors to match brownish background
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Brownish
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));    // Frame border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);  // Window border size

    ImGui::Begin("BackpackGrid",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoScrollbar);

    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            // Set position for this slot - no padding between slots
            float slotX = x * (BACKPACK_SLOT_SIZE - 2);
            float slotY = y * (BACKPACK_SLOT_SIZE - 2);

            ImGui::SetCursorPos(ImVec2(slotX, slotY));

            // Create a selectable area for the slot
            ImGui::PushID(y * GRID_WIDTH + x);

            // Brownish style for slots
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.25f, 0.15f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.3f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));

            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

            bool slotClicked =
                ImGui::Button("", ImVec2(BACKPACK_SLOT_SIZE - 2, BACKPACK_SLOT_SIZE - 2));

            ImGui::PopStyleVar(3);

            if (slotClicked)
            {
                std::cout << "Slot clicked at: " << y << ", " << x << std::endl;
            }

            // Render item in m_backpack slot if it exists
            try
            {
                Item item  = m_backpack.getItemAtTile(y, x);
                int  count = m_backpack.getQuantityAtTile(y, x);

                if (ImGui::IsItemHovered())
                {
                    // Show item description on hover
                    std::string itemName = item.getName();
                    std::replace(itemName.begin(), itemName.end(), '_', ' ');
                    m_hoveredDescription = itemName + " | " + item.getDescription();
                    const auto& effects  = item.getEffects();
                    if (effects.size() > 0)
                    {
                        m_hoveredDescription += " | Effects: ";
                    }
                    for (size_t i = 0; i < effects.size(); ++i)
                    {
                        std::string effectName = effects[i];
                        std::replace(effectName.begin(), effectName.end(), '_', ' ');
                        m_hoveredDescription += effectName + (i < effects.size() - 1 ? ", " : "");
                    }
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    std::string itemType = item.getType();
                    if (itemType == "Weapon" || itemType == "HeadArmor" ||
                        itemType == "BodyArmor" || itemType == "FootArmor" || itemType == "Pendant")
                    {
                        try
                        {
                            m_equipment.equipItemFromBackpack(m_backpack, y, x, itemType);
                        }
                        catch (const InvalidEquipmentTypeException& e)
                        {
                            // Fixed to use m_showErrorPopup
                            m_showErrorPopup = true;
                            m_errorMessage   = "Cannot equip " + itemType + ": " + e.what();
                            std::cerr << "ERROR: " << e.what() << std::endl;
                        }
                        catch (const BackpackOvercapacityException& e)
                        {
                            m_showErrorPopup = true;
                            m_errorMessage = "Your backpack is full! Cannot unequip current item.";
                            std::cerr << "ERROR: " << e.what() << std::endl;
                        }
                        catch (const std::exception& e)
                        {
                            m_showErrorPopup = true;
                            m_errorMessage   = "An error occurred: " + std::string(e.what());
                            std::cerr << "ERROR: " << e.what() << std::endl;
                        }
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

                // Get slot center for proper texture positioning
                // Use window position for proper positioning
                ImVec2 buttonMin = ImGui::GetItemRectMin();
                ImVec2 buttonMax = ImGui::GetItemRectMax();
                ImVec2 buttonCenter((buttonMin.x + buttonMax.x) * 0.5f,
                                    (buttonMin.y + buttonMax.y) * 0.5f);

                // Render the item texture centered in the slot
                sf::Texture* texture =
                    &m_gameContext.GetResourceManager()->GetTexture(item.getItemID());

                if (texture)
                {
                    ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();

                    // Calculate texture size and position to center it
                    ImVec2 textureSize(BACKPACK_SLOT_SIZE - 16, BACKPACK_SLOT_SIZE - 16);
                    ImVec2 texturePos(buttonCenter.x - textureSize.x * 0.5f,
                                      buttonCenter.y - textureSize.y * 0.5f);

                    // Set position and draw the image
                    ImGui::SetCursorScreenPos(texturePos);
                    ImGui::Image(textureId, textureSize);
                }
                else
                {
                    // Fallback to colored button when texture not available
                    ImVec2 buttonSize(BACKPACK_SLOT_SIZE - 16, BACKPACK_SLOT_SIZE - 16);
                    ImVec2 buttonPos(buttonCenter.x - buttonSize.x * 0.5f,
                                     buttonCenter.y - buttonSize.y * 0.5f);

                    ImGui::SetCursorScreenPos(buttonPos);
                    ImGui::PushStyleColor(ImGuiCol_Button, Color::GetTypeColor(item.getType()));
                    ImGui::Button(item.getName().c_str(), buttonSize);
                    ImGui::PopStyleColor();
                }

                // Show stack count for items with count > 1
                if (count > 1)
                {
                    std::string countText = std::to_string(count);
                    ImVec2      textSize  = ImGui::CalcTextSize(countText.c_str());
                    ImVec2      textPos =
                        ImVec2(buttonMax.x - textSize.x - 12, buttonMax.y - textSize.y - 12);

                    // Draw with pink color like in ShopMenu
                    ImGui::GetWindowDrawList()->AddText(
                        textPos, IM_COL32(255, 192, 203, 255), countText.c_str());
                }

                // Rarity indicator in colored format like ShopMenu
                char        r = item.getRarity();
                std::string rarityText(1, r);
                ImVec2      textPos = ImVec2(buttonMin.x + 12, buttonMin.y + 10);

                // Get color based on rarity
                ImVec4 rarityColor    = Color::GetRarityColor(r);
                ImU32  rarityColorU32 = ImGui::ColorConvertFloat4ToU32(rarityColor);

                ImGui::GetWindowDrawList()->AddText(textPos, rarityColorU32, rarityText.c_str());
            }
            catch (const std::exception& e)
            {
                // No item in this slot - check for NOL texture
                ImVec2 buttonMin = ImGui::GetItemRectMin();
                ImVec2 buttonMax = ImGui::GetItemRectMax();
                ImVec2 buttonCenter((buttonMin.x + buttonMax.x) * 0.5f,
                                    (buttonMin.y + buttonMax.y) * 0.5f);

                // Try to render the "NOL" (null) texture if it exists
                sf::Texture* texture = &m_gameContext.GetResourceManager()->GetTexture("NOL");

                if (texture)
                {
                    ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();

                    // Calculate texture size and position to center it
                    ImVec2 textureSize(BACKPACK_SLOT_SIZE - 16, BACKPACK_SLOT_SIZE - 16);
                    ImVec2 texturePos(buttonCenter.x - textureSize.x * 0.5f,
                                      buttonCenter.y - textureSize.y * 0.5f);

                    // Set position and draw the image
                    ImGui::SetCursorScreenPos(texturePos);
                    ImGui::Image(textureId, textureSize);
                }
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

                    try
                    {
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
                                    int qty = m_backpack.getQuantityAtTile(payload->sourceY,
                                                                           payload->sourceX);
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
                                Item src =
                                    m_backpack.getItemAtTile(payload->sourceY, payload->sourceX);

                                // 3b–i) Same item → stack them
                                if (dest.getName() == src.getName())
                                {
                                    int srcQty   = m_backpack.getQuantityAtTile(payload->sourceY,
                                                                              payload->sourceX);
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
                                    int qty = m_backpack.getQuantityAtTile(payload->sourceY,
                                                                           payload->sourceX);
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
                    catch (const BackpackOvercapacityException& e)
                    {
                        m_showErrorPopup = true;
                        m_errorMessage   = "Your backpack is full! Cannot perform this action.";
                        std::cerr << "ERROR: " << e.what() << std::endl;
                    }
                    catch (const std::exception& e)
                    {
                        m_showErrorPopup = true;
                        m_errorMessage   = "An error occurred: " + std::string(e.what());
                        std::cerr << "ERROR: " << e.what() << std::endl;
                    }
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::PopStyleColor(4);
            ImGui::PopID();
        }
    }

    // End the backpack window
    ImGui::End();

    // Pop the backpack grid styles
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(2);
}

void InventoryMenu::RenderEquipment(float startX, float startY)
{
    // m_equipment slot names for display
    const std::string slotNames[EQUIP_SLOTS] = {
        "Weapon", "Head Armor", "Body Armor", "Foot Armor", "Pendant"};

    // m_equipment type identifiers
    const std::string slotTypes[EQUIP_SLOTS] = {
        "Weapon", "HeadArmor", "BodyArmor", "FootArmor", "Pendant"};

    // Calculate precise window dimensions for equipment
    float windowWidth  = EQUIPMENT_SLOT_SIZE;
    float windowHeight = EQUIP_SLOTS * (EQUIPMENT_SLOT_SIZE - 2) + 2;

    // Position the window
    ImVec2 windowPos = ImVec2(startX, startY);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Apply brownish style similar to ShopMenu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    // Set colors to match brownish background
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Brownish
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));    // Frame border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5.0f);  // Window border size
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);    // Rounded corners

    ImGui::Begin("EquipmentGrid",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoScrollbar);

    for (int i = 0; i < EQUIP_SLOTS; i++)
    {
        // Position the slot within the window
        float slotY = i * (EQUIPMENT_SLOT_SIZE - 2);
        ImGui::SetCursorPos(ImVec2(0, slotY));

        // Create a selectable area for the m_equipment slot
        ImGui::PushID(1000 + i);  // Use a different ID range than m_backpack slots

        // Use brownish style for slots
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.25f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.3f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

        bool slotClicked =
            ImGui::Button("", ImVec2(EQUIPMENT_SLOT_SIZE - 2, EQUIPMENT_SLOT_SIZE - 2));

        // Get the button rect for better positioning
        ImVec2 buttonMin = ImGui::GetItemRectMin();
        ImVec2 buttonMax = ImGui::GetItemRectMax();
        ImVec2 buttonCenter((buttonMin.x + buttonMax.x) * 0.5f, (buttonMin.y + buttonMax.y) * 0.5f);

        if (slotClicked)
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
            if (ImGui::IsItemHovered())
            {
                // Show item description on hover with formatted text
                std::string itemName = equippedItem.getName();
                std::replace(itemName.begin(), itemName.end(), '_', ' ');
                m_hoveredDescription = itemName + " | " + equippedItem.getDescription();
                const auto& effects  = equippedItem.getEffects();
                if (effects.size() > 0)
                {
                    m_hoveredDescription += " | Effects: ";
                }
                for (size_t j = 0; j < effects.size(); ++j)
                {
                    std::string effectName = effects[j];
                    std::replace(effectName.begin(), effectName.end(), '_', ' ');
                    m_hoveredDescription += effectName + (j < effects.size() - 1 ? ", " : "");
                }
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                try
                {
                    // Try to unequip this item to the first empty slot
                    std::vector<std::pair<int, int>> emptySlots = m_backpack.findEmptyTile();
                    if (!emptySlots.empty())
                    {
                        // We have at least one empty slot, use that
                        m_equipment.unequipItemToBackpack(
                            m_backpack, emptySlots[0].first, emptySlots[0].second, slotTypes[i]);
                    }
                    else
                    {
                        // No empty slots, just try position 0, 0 and let error handling take over
                        m_equipment.unequipItemToBackpack(m_backpack, 0, 0, slotTypes[i]);
                    }
                }
                catch (const BackpackOvercapacityException& e)
                {
                    m_showErrorPopup = true;
                    m_errorMessage   = "Your backpack is full! Cannot unequip item.";
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }
                catch (const std::exception& e)
                {
                    m_showErrorPopup = true;
                    m_errorMessage   = "An error occurred: " + std::string(e.what());
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }
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

            // Render the equipped item centered in the slot
            sf::Texture* texture =
                &m_gameContext.GetResourceManager()->GetTexture(equippedItem.getItemID());

            if (texture)
            {
                ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();

                // Calculate texture size and position to center it
                ImVec2 textureSize(EQUIPMENT_SLOT_SIZE - 16, EQUIPMENT_SLOT_SIZE - 16);
                ImVec2 texturePos(buttonCenter.x - textureSize.x * 0.5f,
                                  buttonCenter.y - textureSize.y * 0.5f);

                // Set position and draw the image
                ImGui::SetCursorScreenPos(texturePos);
                ImGui::Image(textureId, textureSize);
            }
            else
            {
                // Fallback to colored button when texture not available
                ImVec2 buttonSize(EQUIPMENT_SLOT_SIZE - 16, EQUIPMENT_SLOT_SIZE - 16);
                ImVec2 buttonPos(buttonCenter.x - buttonSize.x * 0.5f,
                                 buttonCenter.y - buttonSize.y * 0.5f);

                ImGui::SetCursorScreenPos(buttonPos);
                ImGui::PushStyleColor(ImGuiCol_Button, Color::GetTypeColor(equippedItem.getType()));
                ImGui::Button(equippedItem.getName().c_str(), buttonSize);
                ImGui::PopStyleColor();
            }

            // Draw rarity indicator
            char        r = equippedItem.getRarity();
            std::string rarityText(1, r);
            ImVec2      textPos = ImVec2(buttonMin.x + 12, buttonMin.y + 10);

            // Get color based on rarity
            ImVec4 rarityColor    = Color::GetRarityColor(r);
            ImU32  rarityColorU32 = ImGui::ColorConvertFloat4ToU32(rarityColor);

            ImGui::GetWindowDrawList()->AddText(textPos, rarityColorU32, rarityText.c_str());
        }
        else
        {
            // Empty equipment slot - add centered text with the slot type
            // Get a NOL texture if it exists
            sf::Texture* texture = &m_gameContext.GetResourceManager()->GetTexture("NOL");

            if (texture)
            {
                ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();

                // Calculate texture size and position to center it
                ImVec2 textureSize(EQUIPMENT_SLOT_SIZE - 16, EQUIPMENT_SLOT_SIZE - 16);
                ImVec2 texturePos(buttonCenter.x - textureSize.x * 0.5f,
                                  buttonCenter.y - textureSize.y * 0.5f);

                // Set position and draw the image
                ImGui::SetCursorScreenPos(texturePos);
                ImGui::Image(textureId, textureSize);
            }

            // Calculate text size for centering
            // Use the displayable name (shorter and more readable)
            ImVec2 textSize = ImGui::CalcTextSize(slotNames[i].c_str());

            // Calculate position to center the text
            ImVec2 textPos =
                ImVec2(buttonCenter.x - textSize.x * 0.5f, buttonCenter.y - textSize.y * 0.5f);

            // Draw white text with black outline for visibility
            // First draw black outline by drawing text 5 times with slight offsets
            ImU32 blackColor = IM_COL32(0, 0, 0, 255);
            ImU32 whiteColor = IM_COL32(255, 255, 255, 255);

            // Draw outline
            for (int dx = -1; dx <= 1; dx += 1)
            {
                for (int dy = -1; dy <= 1; dy += 1)
                {
                    if (dx != 0 || dy != 0)
                    {  // Skip the center position
                        ImVec2 outlinePos = ImVec2(textPos.x + dx, textPos.y + dy);
                        ImGui::GetForegroundDrawList()->AddText(
                            outlinePos, blackColor, slotNames[i].c_str());
                    }
                }
            }

            // Draw the actual text in white
            ImGui::GetForegroundDrawList()->AddText(textPos, whiteColor, slotNames[i].c_str());
        }

        // Create drop target for m_equipment slots
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* imgui_payload = ImGui::AcceptDragDropPayload("INVENTORY_ITEM"))
            {
                const DragDropPayload* payload = (const DragDropPayload*)imgui_payload->Data;
                if (!payload->fromEquipment)
                {
                    // Try to equip from backpack to this slot
                    try
                    {
                        m_equipment.equipItemFromBackpack(
                            m_backpack, payload->sourceY, payload->sourceX, slotTypes[i]);
                    }
                    catch (const InvalidEquipmentTypeException& e)
                    {
                        m_showErrorPopup = true;
                        try
                        {
                            Item item = m_backpack.getItemAtTile(payload->sourceY, payload->sourceX);
                            std::string itemType = item.getType();

                            if (itemType == "HeadArmor" || itemType == "BodyArmor" ||
                                itemType == "FootArmor")
                                itemType.insert(4, " ");

                            m_errorMessage =
                                "Cannot equip " + item.getType() + " in " + slotNames[i] + " slot.";
                        }
                        catch (const std::exception&)
                        {
                            // Fallback if we can't access the item
                            m_errorMessage =
                                "Cannot equip item: wrong item type for " + slotNames[i] + " slot.";
                        }
                        std::cerr << "ERROR: " << e.what() << std::endl;
                    }
                    catch (const BackpackOvercapacityException& e)
                    {
                        m_showErrorPopup = true;
                        m_errorMessage   = "Your backpack is full! Cannot unequip current item.";
                        std::cerr << "ERROR: " << e.what() << std::endl;
                    }
                    catch (const std::exception& e)
                    {
                        m_showErrorPopup = true;
                        m_errorMessage   = "An error occurred: " + std::string(e.what());
                        std::cerr << "ERROR: " << e.what() << std::endl;
                    }
                }
                else
                {
                    // Equipment to equipment swap - show error popup
                    m_showErrorPopup = true;
                    m_errorMessage   = "Equipment to equipment transfers are not supported.";
                    std::cerr << "ERROR: Equipment to equipment transfer not implemented"
                              << std::endl;
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(3);  // pop FrameBorderSize, FramePadding, FrameRounding
        ImGui::PopID();
    }

    // End the equipment window
    ImGui::End();

    // Pop the equipment grid styles
    ImGui::PopStyleVar(6);
    ImGui::PopStyleColor(2);
}