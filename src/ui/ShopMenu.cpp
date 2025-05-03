#include "ui/ShopMenu.hpp"

#include <cstring>

#include "parser/ItemConfigParser.hpp"
#include "parser/ShopConfigParser.hpp"
#include "rng/rng.hpp"

ShopMenu::ShopMenu(GameContext& gameContext)
    : m_gameContext(gameContext),
      m_shop(*gameContext.GetShop()),
      m_backpack(*gameContext.GetBackpack()),
      m_playerID(gameContext.GetCharacterId())
{}

void ShopMenu::Render()
{
    // Calculate exact dimensions - no extra margins or padding
    int visibleRows = 4;  // Show exactly 6 rows at a time

    // Calculate precise window dimensions - no extra padding
    float windowWidth  = GRID_WIDTH * SLOT_SIZE + 13;
    float windowHeight = visibleRows * SLOT_SIZE;

    // Center the window horizontally better (moved more to the right)
    ImVec2 windowPos = ImVec2(425.0f, 65.0f);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Disable window padding completely to eliminate spacing issues
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    // Set colors to match brownish background
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Brownish
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));  // Transparent
                                                                                  // background
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab,
                          ImVec4(0.55f, 0.30f, 0.20f, 0.9f));  // Lighter brown, semi-transparent
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.65f, 0.40f, 0.25f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.70f, 0.45f, 0.30f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));  // Frame border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);  // Window border size
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);    // Rounded corners

    // Create ONE single window with scrolling - no hidden containers
    ImGui::Begin("ShopBuy",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_AlwaysVerticalScrollbar);

    RNG rng;
    int idx = 0;

    // Get filtered stock first to avoid repeated calls
    std::vector<Shop::StockEntry> filteredStock = m_shop.filterStock("All");

    // Calculate exactly how many rows we need based on actual stock
    GRID_HEIGHT    = filteredStock.size() / GRID_WIDTH;
    int extraItems = filteredStock.size() % GRID_WIDTH;
    if (extraItems > 0)
        GRID_HEIGHT++;

    // Draw the grid directly in this window
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            // Calculate position for this slot - exact positions with no padding
            float slotX = x * SLOT_SIZE;
            float slotY = y * SLOT_SIZE;

            // Set the cursor position for this slot
            ImGui::SetCursorPos(ImVec2(slotX, slotY));

            // Generate a unique ID for this slot
            ImGui::PushID(y * GRID_WIDTH + x);

            Item item;
            int  price       = 0;
            int  stock       = 0;
            bool isEmptySlot = false;

            // Get item data for this slot - Now with proper bounds checking
            try
            {
                if (idx < filteredStock.size())
                {
                    auto entry = filteredStock[idx];
                    idx++;

                    item  = std::get<0>(entry);
                    price = std::get<1>(entry);
                    stock = std::get<2>(entry);

                    // Check if this is an empty item
                    isEmptySlot = (item.getItemID().empty() || item.getRarity() == ' ');
                }
                else
                {
                    isEmptySlot = true;
                    // Use empty item with proper itemID for texture
                    item = Item();
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception in ShopMenu::Render: " << e.what() << std::endl;
                isEmptySlot = true;
                // Use empty item with proper itemID for texture
                item = Item();
            }

            // Apply different styles for empty vs. filled slots
            if (isEmptySlot)
            {
                // Empty slot - draw just a plain background with no hover effects
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Background
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Same as background -
                                                                           // no hover effect
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Same as background -
                                                                           // no active effect
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));  // Border
            }
            else
            {
                // Normal item slot with hover effects
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Background
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      ImVec4(0.2f, 0.5f, 0.2f, 1.0f));  // Green hover
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      ImVec4(0.25f, 0.6f, 0.25f, 1.0f));  // Brighter green active
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));  // Border
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);       // Add 1px border
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));  // Ensure consistent
                                                                            // padding
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);  // Round all slot corners

            // Create the slot button with a slightly smaller size to show borders
            bool clicked = ImGui::Button("", ImVec2(SLOT_SIZE - 2, SLOT_SIZE - 2));

            // Only process clicks for non-empty slots
            if (clicked && !isEmptySlot)
            {
                std::cout << "Clicked on slot " << x << "," << y << std::endl;
                // Handle item purchase or selection logic here

                /*
                Character* player =
                    m_gameContext.GetUnitManager()->GetUnitOfType<Character>(m_playerID);
                m_gameContext.GetShop()->buy(*player,
                                             m_backpack,
                                             item,
                                             price,
                                             1);
                */
            }

            // Get the position where we just drew the button
            ImVec2 buttonPos = ImGui::GetItemRectMin();

            // Only render item details if there's a valid item
            // Position where we'll draw the item texture/button
            // Calculate the center of the button for proper alignment
            ImVec2 buttonCenter =
                ImVec2(buttonPos.x + (SLOT_SIZE - 2) / 2.0f, buttonPos.y + (SLOT_SIZE - 2) / 2.0f);

            // Always render the item image/texture (even for empty slots)
            sf::Texture* texture = &m_gameContext.GetResourceManager()->GetTexture(item.getItemID());
            if (texture)
            {
                ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();
                ImVec2      imageSize(SLOT_SIZE - 16, SLOT_SIZE - 16);
                ImVec2      imagePos(buttonCenter.x - imageSize.x / 2.0f,
                                buttonCenter.y - imageSize.y / 2.0f);
                ImGui::SetCursorScreenPos(imagePos);
                ImGui::Image(textureId, imageSize);

                // Only show item details for non-empty slots
                if (!isEmptySlot)
                {
                    // Rarity label (top-left)
                    char r = item.getRarity();
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 7, buttonPos.y + 5));
                    ImGui::Text("%c", r);

                    // Price label (bottom-left)
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 7, buttonPos.y + SLOT_SIZE - 20));
                    ImGui::Text("%d", price);

                    // Stock label (bottom-right)
                    std::string stockText     = std::to_string(stock);
                    ImVec2      stockTextSize = ImGui::CalcTextSize(stockText.c_str());
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + SLOT_SIZE - stockTextSize.x - 11,
                                                     buttonPos.y + SLOT_SIZE - 20));
                    ImGui::Text("%d", stock);
                }
            }
            else
            {
                ImVec2 itemButtonSize(SLOT_SIZE - 16, SLOT_SIZE - 16);
                ImVec2 itemButtonPos(buttonCenter.x - itemButtonSize.x / 2.0f,
                                     buttonCenter.y - itemButtonSize.y / 2.0f);
                ImGui::SetCursorScreenPos(itemButtonPos);
                ImGui::Button(item.getName().c_str(), itemButtonSize);
                ImGui::PopStyleColor();

                // Only show item details for non-empty slots
                if (!isEmptySlot)
                {
                    // Rarity label (top-left)
                    char r = item.getRarity();
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 7, buttonPos.y + 5));
                    ImGui::Text("%c", r);

                    // Price label (bottom-left)
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 7, buttonPos.y + SLOT_SIZE - 20));
                    ImGui::Text("%d", price);

                    // Stock label (bottom-right)
                    std::string stockText     = std::to_string(stock);
                    ImVec2      stockTextSize = ImGui::CalcTextSize(stockText.c_str());
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + SLOT_SIZE - stockTextSize.x - 11,
                                                     buttonPos.y + SLOT_SIZE - 20));
                    ImGui::Text("%d", stock);
                }
            }

            // Pop style and ID
            ImGui::PopStyleVar(3);    // Pop the 3 style vars (border size, padding, rounding)
            ImGui::PopStyleColor(4);  // Pop the 4 colors we pushed for this slot
            ImGui::PopID();

            // IMPORTANT: Don't use SameLine() - it adds spacing
            // Instead, rely on the exact positioning with SetCursorPos
        }
    }

    // Force exact content height with one final element
    ImGui::SetCursorPos(ImVec2(0, GRID_HEIGHT * SLOT_SIZE - 1));  // Exact position
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);               // Invisible
    ImGui::Button("##endspacer", ImVec2(SLOT_SIZE, 1));           // Minimal size
    ImGui::PopStyleVar();

    // Restore style values
    ImGui::End();
    ImGui::PopStyleVar(6);    // Pop the 6 style vars (4 at beginning + window border + window
                              // rounding)
    ImGui::PopStyleColor(6);  // Pop the 6 color styles we pushed
}