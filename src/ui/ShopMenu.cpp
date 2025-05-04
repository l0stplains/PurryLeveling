#include "ui/ShopMenu.hpp"

#include <cstring>

#include "parser/ItemConfigParser.hpp"
#include "parser/ShopConfigParser.hpp"
#include "rng/rng.hpp"

ShopMenu::ShopMenu(GameContext& gameContext)
    : m_gameContext(gameContext),
      m_shop(*gameContext.GetShop()),
      m_backpack(*gameContext.GetBackpack()),
      m_playerID(gameContext.GetCharacterId()),
      m_activeCategories(),
      m_currentSort(""),
      m_sortState("un"),
      m_isSellingMode(false),
      m_hoveredItem(),
      m_showModal(false),
      m_purchaseQty(1),
      m_totalPrice(0),
      m_showError(false),
      m_errorMessage("")
{
    RefreshStock();
}

void ShopMenu::Render()
{
    RenderGoldDisplay();
    RenderControlPanel();
    RenderItemDescription();
    RenderGrid();

    // Open popup when m_showModal is true
    if (m_showModal)
    {
        std::string modalTitle = m_isSellingMode ? "Sell Item" : "Buy Item";
        ImGui::OpenPopup(modalTitle.c_str());
    }

    RenderModal();
}

void ShopMenu::ShowError(const std::string& message)
{
    m_errorMessage = message;
    m_showError    = true;
}

void ShopMenu::ClearError()
{
    m_errorMessage = "";
    m_showError    = false;
}

bool ShopMenu::IsModalOpen() const
{
    return m_showModal;
}

void ShopMenu::RenderModal()
{
    // Center the modal window - compact size
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(320, 390));

    // Style the modal window
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));  // Background
                                                                                       // overlay
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    std::string modalTitle = m_isSellingMode ? "Sell Item" : "Buy Item";
    if (ImGui::BeginPopupModal(modalTitle.c_str(),
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        const float spacing        = 4.0f;  // Standard spacing value
        const float sectionSpacing = 8.0f;  // Smaller spacing between sections

        ImGui::Dummy(ImVec2(0, spacing));

        // Left side - Smaller texture frame
        ImGui::BeginGroup();
        float slotSize = SLOT_SIZE * 0.7f;

        // Create slot-style frame
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));

        ImGui::PushID("ItemSlot");
        ImGui::Button("", ImVec2(slotSize - 28, slotSize - 28));

        // Get button position for texture placement
        ImVec2 slotPos = ImGui::GetItemRectMin();
        ImVec2 slotCenter =
            ImVec2(slotPos.x + (slotSize - 28) / 2.0f, slotPos.y + (slotSize - 28) / 2.0f);

        // Render item texture
        sf::Texture* texture =
            &m_gameContext.GetResourceManager()->GetTexture(m_selectedItem.getItemID());
        if (texture)
        {
            ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();
            ImVec2      imageSize(slotSize - 38, slotSize - 38);
            ImVec2 imagePos(slotCenter.x - imageSize.x / 2.0f, slotCenter.y - imageSize.y / 2.0f);
            ImGui::SetCursorScreenPos(imagePos);
            ImGui::Image(textureId, imageSize);
        }

        ImGui::PopID();
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);
        ImGui::EndGroup();

        ImGui::SameLine(slotSize);

        // Right side - Compact item info
        ImGui::BeginGroup();

        // Item name
        std::string itemName = m_selectedItem.getName();
        std::replace(itemName.begin(), itemName.end(), '_', ' ');

        ImGui::SetWindowFontScale(1.2f);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", itemName.c_str());
        ImGui::SetWindowFontScale(1.1f);
        ImGui::Dummy(ImVec2(0, spacing * 0.5f));

        // Type and Rarity
        ImGui::Text("Type: %s", m_selectedItem.getType().c_str());
        ImGui::Dummy(ImVec2(0, spacing * 0.5f));

        ImGui::Text("Rarity:");
        ImGui::SameLine();
        ImGui::TextColored(
            GetRarityColor(m_selectedItem.getRarity()), "%c", m_selectedItem.getRarity());
        ImGui::SetWindowFontScale(1.0f);
        ImGui::EndGroup();

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Description
        ImGui::TextWrapped("%s", m_selectedItem.getDescription().c_str());

        // Effects section
        std::vector<std::shared_ptr<Effect>> effects = m_selectedItem.getEffects();
        ImGui::Dummy(ImVec2(0, spacing));
        ImGui::Text("Effects:");
        if (!effects.empty())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
            for (const auto& effect : effects)
            {
                ImGui::Text("- %s", effect->GetName().c_str());
            }
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::Text("None");
        }

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Center-aligned quantity controls
        float contentWidth  = ImGui::GetContentRegionAvail().x;
        float controlsWidth = 110.0f;
        float startX        = (contentWidth - controlsWidth) / 2.0f;

        ImGui::SetCursorPosX(startX);

        // Decrease button
        bool canDecrease = m_purchaseQty > 1;
        ImGui::BeginDisabled(!canDecrease);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

        ImVec4 buttonColor = canDecrease ? ImVec4(0.25f, 0.25f, 0.25f, 1.0f)
                                         : ImVec4(0.25f, 0.25f, 0.25f, 0.5f);
        ImVec4 hoverColor  = canDecrease ? ImVec4(0.35f, 0.35f, 0.35f, 1.0f) : buttonColor;
        ImVec4 borderColor = canDecrease ? ImVec4(0.4f, 0.4f, 0.4f, 1.0f)
                                         : ImVec4(0.4f, 0.4f, 0.4f, 0.5f);

        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);

        if (ImGui::Button("-", ImVec2(30, 30)))
        {
            m_purchaseQty--;
            ClearError();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);
        ImGui::EndDisabled();

        ImGui::SameLine();

        // Quantity input
        ImGui::SetNextItemWidth(50);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 6));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

        bool valueChanged = ImGui::InputInt("##qty", &m_purchaseQty, 0, 0);

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);

        ImGui::SameLine();

        // Increase button
        bool canIncrease = m_purchaseQty < m_selectedQtyInStock;
        ImGui::BeginDisabled(!canIncrease);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

        buttonColor = canIncrease ? ImVec4(0.25f, 0.25f, 0.25f, 1.0f)
                                  : ImVec4(0.25f, 0.25f, 0.25f, 0.5f);
        hoverColor  = canIncrease ? ImVec4(0.35f, 0.35f, 0.35f, 1.0f) : buttonColor;
        borderColor = canIncrease ? ImVec4(0.4f, 0.4f, 0.4f, 1.0f) : ImVec4(0.4f, 0.4f, 0.4f, 0.5f);

        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, hoverColor);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);

        if (ImGui::Button("+", ImVec2(30, 30)))
        {
            m_purchaseQty++;
            ClearError();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);
        ImGui::EndDisabled();

        ImGui::Dummy(ImVec2(0, spacing));

        // Total price display
        m_totalPrice = m_selectedPrice * m_purchaseQty;

        ImGui::SetCursorPosX(startX);
        ImVec2 goldSize = ImVec2(125.0f, 25.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.2f, 0.0f, 0.7f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);

        ImGui::BeginChild("TotalGold", goldSize, true);
        ImGui::SetCursorPos(ImVec2(8, 4));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

        if (m_purchaseQty <= m_selectedQtyInStock && m_purchaseQty > 0)
        {
            ImGui::Text("$%d", m_totalPrice);
        }
        else
        {
            ImGui::Text("%s", "???");
        }

        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Bottom buttons - centered
        float  buttonWidth = 100.0f;
        ImVec2 buttonSize(buttonWidth, 35.0f);
        float  buttonStartX = startX - 40.0f;

        ImGui::SetCursorPosX(buttonStartX);

        // Buy/Sell button
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.3f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.4f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));

        std::string actionText = m_isSellingMode ? "Sell" : "Buy";
        if (ImGui::Button(actionText.c_str(), buttonSize))
        {
            try
            {
                // Check if transaction is valid
                Character* player =
                    m_gameContext.GetUnitManager()->GetUnitOfType<Character>(m_playerID);
                if (m_purchaseQty > m_selectedQtyInStock)
                {
                    ShowError("Exceeds available stock");
                }
                else if (m_purchaseQty <= 0)
                {
                    ShowError("Invalid quantity");
                }
                else if (!m_isSellingMode && player->GetGold() < m_totalPrice)
                {
                    ShowError("Not enough gold");
                }
                else
                {
                    if (m_isSellingMode)
                    {
                        m_shop.sell(*player, m_backpack, m_selectedItem, m_totalPrice, m_purchaseQty);
                    }
                    else
                    {
                        Item itemToBuy = m_selectedItem;
                        m_shop.buy(*player, m_backpack, itemToBuy, m_totalPrice, m_purchaseQty);
                    }

                    // Transaction successful
                    m_showModal = false;
                    RefreshStock();
                    ImGui::CloseCurrentPopup();  // Close the popup
                }
            }
            catch (const std::exception& e)
            {
                ShowError(std::string("Transaction failed: ") + e.what());
            }
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        ImGui::SameLine();

        // Cancel button
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

        if (ImGui::Button("Cancel", buttonSize))
        {
            m_showModal = false;
            ClearError();
            ImGui::CloseCurrentPopup();  // Close the popup
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        // Error message
        if (m_showError)
        {
            ImGui::Dummy(ImVec2(0, spacing));
            ImGui::SetCursorPosX(
                (contentWidth - ImGui::CalcTextSize(m_errorMessage.c_str()).x + 10.0f) / 2.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
            ImGui::Text("%s", m_errorMessage.c_str());
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }

    // Pop the modal window styles
    ImGui::PopStyleVar(5);    // WindowBorderSize, WindowRounding, WindowPadding, ItemSpacing,
                              // FramePadding
    ImGui::PopStyleColor(3);  // WindowBg, Border, ModalWindowDimBg
}

void ShopMenu::RenderGoldDisplay()
{
    // Get player
    int gold = m_gameContext.GetUnitManager()->GetUnitOfType<Character>(m_playerID)->GetGold();

    // Set position for gold display (top-left area based on image)
    ImVec2 goldPos = ImVec2(90.0f, 45.0f);  // Moved up slightly from 45 to 30
    ImGui::SetNextWindowPos(goldPos);
    ImGui::SetNextWindowSize(ImVec2(265.0f, 50.0f));  // Increased height from 40 to 50

    // Style for gold display
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 5));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.0f, 0.7f));  // Dark green
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));  // Bright green border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    ImGui::Begin("Gold",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);

    // Bigger font for gold display
    ImGui::SetWindowFontScale(1.5f);

    ImGui::SetCursorPos(ImVec2(20, 13));  // Adjusted for larger font
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // Yellow text
    ImGui::Text("$%d", gold);
    ImGui::PopStyleColor();

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

void ShopMenu::RenderControlPanel()
{
    // Control panel position - adjusted for more compact layout
    ImVec2 controlPos = ImVec2(90.0f, 120.0f);
    ImGui::SetNextWindowPos(controlPos);
    ImGui::SetNextWindowSize(ImVec2(265.0f, 380.0f));  // Reduced height from 440 to 370

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.9f));  // Dark grey
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));  // Light grey border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);                 // Rounded corners

    ImGui::Begin("Controls",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse);  // No scrolling

    // Set a larger font for the text elements
    ImGui::SetWindowFontScale(1.2f);  // 30% larger

    // Buy/Sell buttons
    ImVec2 buttonSize = ImVec2(118.0f, 40.0f);  // Increased for larger font

    // Buy button - with border and rounding
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    if (m_isSellingMode == false)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.3f, 0.0f, 1.0f));  // Dark green
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));  // Bright green
                                                                                 // border
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));  // Dark grey
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));  // Light grey
                                                                                 // border
    }
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

    if (ImGui::Button("Buy", buttonSize))
    {
        if (m_isSellingMode)
        {
            m_isSellingMode = false;
            m_activeCategories.clear();  // Reset filters
            m_currentSort = "";          // Reset sort
            m_sortState   = "un";
            RefreshStock();
        }
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    ImGui::SameLine();

    // Sell button - with border and rounding
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    if (m_isSellingMode == true)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.3f, 0.0f, 1.0f));  // Dark green
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));  // Bright green
                                                                                 // border
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));  // Dark grey
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));  // Light grey
                                                                                 // border
    }
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

    if (ImGui::Button("Sell", buttonSize))
    {
        if (!m_isSellingMode)
        {
            m_isSellingMode = true;
            m_activeCategories.clear();  // Reset filters
            m_currentSort = "";          // Reset sort
            m_sortState   = "un";
            RefreshStock();
        }
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    // Add more spacing around first separator
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // Add "Filter by" text with spacing below
    ImGui::Text("Filter by");
    ImGui::Spacing();
    ImGui::Spacing();

    // Filter buttons - Now in 3 rows x 2 columns layout
    const char* categories[] = {"Potion", "Weapon", "HeadArmor", "BodyArmor", "FootArmor", "Pendant"};
    const char* categoryValues[] = {
        "Potion", "Weapon", "HeadArmor", "BodyArmor", "FootArmor", "Pendant"};

    ImVec2 filterButtonSize = ImVec2(118.0f, 40.0f);  // Wider buttons to fit 2 per row

    for (int i = 0; i < 6; i++)
    {
        // Add border and rounding for filter buttons
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

        bool isActive =
            std::find(m_activeCategories.begin(), m_activeCategories.end(), categoryValues[i]) !=
            m_activeCategories.end();

        if (isActive)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.3f, 0.0f, 1.0f));  // Dark green
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));  // Bright green
                                                                                     // border
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));  // Dark grey
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));  // Light grey
                                                                                     // border
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

        if (ImGui::Button(categories[i], filterButtonSize))
        {
            // Toggle individual category
            auto it =
                std::find(m_activeCategories.begin(), m_activeCategories.end(), categoryValues[i]);
            if (it != m_activeCategories.end())
            {
                m_activeCategories.erase(it);
            }
            else
            {
                m_activeCategories.push_back(categoryValues[i]);
            }
            RefreshStock();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        // Place 2 buttons per row
        if ((i + 1) % 2 != 0 && i < 5)
        {
            ImGui::SameLine();
        }
        else if (i < 5)
        {
            ImGui::Spacing();
            ImGui::Spacing();  // Add more vertical spacing between rows
        }
    }

    // Add "All" as checkbox below the filter grid with proper spacing
    ImGui::Spacing();
    ImGui::Spacing();

    bool allActive = (m_activeCategories.size() == 6);
    if (ImGui::Checkbox("All", &allActive))
    {
        if (allActive)
        {
            // Activate all categories
            m_activeCategories.clear();
            for (int j = 0; j < 6; j++)
            {
                m_activeCategories.push_back(categoryValues[j]);
            }
        }
        else
        {
            // Deactivate all categories
            m_activeCategories.clear();
        }
        RefreshStock();
    }

    // Add more spacing around second separator
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // Sort buttons with spacing after text
    ImGui::Text("Sort by");
    ImGui::Spacing();
    ImGui::Spacing();

    const char* sortOptions[] = {"Rarity", "Price", "Stock"};

    ImGui::BeginGroup();
    for (int i = 0; i < 3; i++)
    {
        if (i > 0)
            ImGui::SameLine();

        ImVec2 sortButtonSize = ImVec2(76.0f, 40.0f);  // Adjusted for larger font

        // Add border and rounding for sort buttons
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

        ImVec4 buttonColor;
        ImVec4 buttonHoverColor;
        ImVec4 borderColor;

        if (m_currentSort == sortOptions[i])
        {
            if (m_sortState == "desc")
            {
                // Red for descending
                buttonColor      = ImVec4(0.3f, 0.0f, 0.0f, 1.0f);  // Dark red
                buttonHoverColor = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
                borderColor      = ImVec4(0.7f, 0.0f, 0.0f, 1.0f);  // Bright red border
            }
            else if (m_sortState == "asc")
            {
                // Green for ascending
                buttonColor      = ImVec4(0.0f, 0.3f, 0.0f, 1.0f);  // Dark green
                buttonHoverColor = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);
                borderColor      = ImVec4(0.0f, 0.7f, 0.0f, 1.0f);  // Bright green border
            }
            else
            {
                // Default color for unactivated
                buttonColor      = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);  // Dark grey
                buttonHoverColor = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
                borderColor      = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);  // Light grey border
            }
        }
        else
        {
            // Default color for inactive sort options
            buttonColor      = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);  // Dark grey
            buttonHoverColor = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
            borderColor      = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);  // Light grey border
        }

        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);

        if (ImGui::Button(sortOptions[i], sortButtonSize))
        {
            if (m_currentSort == sortOptions[i])
            {
                // Cycle through states: un -> desc -> asc -> un
                if (m_sortState == "un")
                {
                    m_sortState = "desc";
                }
                else if (m_sortState == "desc")
                {
                    m_sortState = "asc";
                }
                else if (m_sortState == "asc")
                {
                    m_sortState   = "un";
                    m_currentSort = "";  // No sorting active
                }
            }
            else
            {
                // Set this as active sort and start with descending
                m_currentSort = sortOptions[i];
                m_sortState   = "desc";
            }
            RefreshStock();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);
    }
    ImGui::EndGroup();

    ImGui::End();
    ImGui::PopStyleVar(4);  // Pop window style vars
    ImGui::PopStyleColor(2);
}

void ShopMenu::RenderItemDescription()
{
    // Move description box up to use the saved space
    ImVec2 descPos = ImVec2(90.0f, 525.0f);  // Moved up from 565 to 495
    ImGui::SetNextWindowPos(descPos);
    ImGui::SetNextWindowSize(ImVec2(265.0f, 130.0f));  // Increased height to 95

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25f, 0.15f, 0.1f, 0.9f));  // Dark brown
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    ImGui::Begin("Description",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);

    // Keep normal font scale (1.0f) for description box
    ImGui::SetWindowFontScale(1.0f);

    if (!m_hoveredItem.getItemID().empty() && !m_hoveredItem.isNull())
    {
        // Item name
        // TODO: Clear underscores from name to space

        std::string itemName = m_hoveredItem.getName();
        std::replace(itemName.begin(), itemName.end(), '_', ' ');

        // Set color based on item type
        ImVec4 itemColor = GetItemColor(m_hoveredItem.getName(), m_hoveredItem.getType());
        ImGui::PushStyleColor(ImGuiCol_Text, itemColor);
        ImGui::Text("%s", itemName.c_str());
        ImGui::PopStyleColor();

        // Effect
        std::vector<std::shared_ptr<Effect>> effects = m_hoveredItem.getEffects();

        // Display effects in format "Effect: <effect_1>, <effect_2>, ..."
        std::string effectText;
        for (size_t i = 0; i < effects.size(); ++i)
        {
            if (i > 0)
                effectText += ", ";
            effectText += effects[i]->GetName();
        }

        if (!effectText.empty())
        {
            // use Yellow color for effects
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // Yellow
            ImGui::Text("Effects: %s", effectText.c_str());
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::Text("Effects: None");
        }

        // Description
        ImGui::TextWrapped("%s", m_hoveredItem.getDescription().c_str());
    }
    else
    {
        // Empty or no item being hovered
        ImGui::TextDisabled("Hover over an item to see details");
    }

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

void ShopMenu::RefreshStock()
{
    const int TOTAL_VISIBLE_SLOTS = 16;  // 4x4 grid

    if (m_isSellingMode)
    {
        // Get items from backpack for selling
        m_stock = m_shop.getBackpackStockForSelling(m_backpack, m_activeCategories);
    }
    else
    {
        // Get filtered and sorted stock from shop
        if (m_sortState == "un" || m_currentSort.empty())
        {
            // No sorting - just filter by categories
            m_stock = m_shop.filterStockByCategories(m_activeCategories);
        }
        else
        {
            // Sort with specified direction
            m_stock = m_shop.getSortedStock(m_activeCategories, m_currentSort, m_sortState == "desc");
        }
    }

    // Fill with empty slots if needed
    int currentSize = m_stock.size();
    if (currentSize < TOTAL_VISIBLE_SLOTS)
    {
        // Create empty items to fill the grid
        for (int i = currentSize; i < TOTAL_VISIBLE_SLOTS; i++)
        {
            Item             emptyItem;  // Default empty item
            Shop::StockEntry emptyEntry = std::make_tuple(emptyItem, 0, 0);
            m_stock.push_back(emptyEntry);
        }
    }
}

void ShopMenu::RenderGrid()
{
    // Calculate exact dimensions - no extra margins or padding
    int visibleRows = 4;  // Show exactly 4 rows at a time

    // Calculate precise window dimensions - no extra padding
    float windowWidth  = GRID_WIDTH * SLOT_SIZE + 13;
    float windowHeight = visibleRows * SLOT_SIZE;

    // Center the window horizontally better (moved more to the right)
    ImVec2 windowPos = ImVec2(425.0f, 75.0f);
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

    // Create ONE single window with scrolling
    ImGui::Begin("ShopGrid",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_AlwaysVerticalScrollbar);

    RNG rng;
    int idx = 0;

    // Calculate exactly how many rows we need based on actual stock
    GRID_HEIGHT    = m_stock.size() / GRID_WIDTH;
    int extraItems = m_stock.size() % GRID_WIDTH;
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
            int  quantity    = 0;
            bool isEmptySlot = false;

            // Get item data for this slot - Now with proper bounds checking
            try
            {
                if (idx < m_stock.size())
                {
                    auto entry = m_stock[idx];
                    idx++;

                    item     = std::get<0>(entry);
                    price    = std::get<1>(entry);
                    quantity = std::get<2>(entry);

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
                std::cerr << "Exception in ShopMenu::RenderGrid: " << e.what() << std::endl;
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

            // Check for hover
            if (ImGui::IsItemHovered() && !isEmptySlot)
            {
                m_hoveredItem = item;
            }

            // Only process clicks for non-empty slots
            if (clicked && !isEmptySlot)
            {
                std::cout << "Clicked on slot " << x << "," << y << std::endl;

                // Set up modal data
                m_selectedItem       = item;
                m_selectedPrice      = price;
                m_selectedQuantity   = quantity;
                m_selectedQtyInStock = quantity;
                m_purchaseQty        = 1;      // Reset to default
                m_totalPrice         = price;  // Initialize with price for 1 item
                m_showModal          = true;

                // If buying mode and player doesn't have enough gold, still show modal but they'll
                // get error on purchase
                Character* player =
                    m_gameContext.GetUnitManager()->GetUnitOfType<Character>(m_playerID);
                if (!m_isSellingMode && player->GetGold() < price)
                {
                    std::cout << "Warning: Player has insufficient gold for this item" << std::endl;
                }
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
                    // Temporarily increase font size for item details
                    ImGui::SetWindowFontScale(1.1f);

                    // Rarity label (top-left with padding) - Different colors for different
                    // rarities
                    char r = item.getRarity();
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 12, buttonPos.y + 10));  // More
                                                                                            // padding
                                                                                            // from
                                                                                            // corner

                    // Set color based on rarity
                    ImVec4 rarityColor;
                    switch (r)
                    {
                        case 'S':
                            rarityColor = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
                            break;  // Light blue for S
                        case 'A':
                            rarityColor = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
                            break;  // Light green for A
                        case 'B':
                            rarityColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
                            break;  // Orange for B
                        case 'C':
                            rarityColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f);
                            break;  // Light red for C
                        case 'D':
                            rarityColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                            break;  // Light grey for D
                        case 'E':
                            rarityColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                            break;  // Darker grey for E
                        default:
                            rarityColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                            break;  // White for others
                    }

                    ImGui::PushStyleColor(ImGuiCol_Text, rarityColor);
                    ImGui::Text("%c", r);
                    ImGui::PopStyleColor();

                    // Price label (bottom-left with padding) - Yellow color with $
                    ImGui::SetCursorScreenPos(
                        ImVec2(buttonPos.x + 12, buttonPos.y + SLOT_SIZE - 26));  // More padding
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // Yellow
                                                                                           // color
                    ImGui::Text("$%d", price);  // Added $ before price
                    ImGui::PopStyleColor();

                    // Quantity label (bottom-right with padding) - Pink color
                    std::string quantityText     = std::to_string(quantity);
                    ImVec2      quantityTextSize = ImGui::CalcTextSize(quantityText.c_str());
                    ImGui::SetCursorScreenPos(
                        ImVec2(buttonPos.x + SLOT_SIZE - quantityTextSize.x - 17,  // More padding
                                                                                   // from edge
                               buttonPos.y + SLOT_SIZE - 26));  // More padding from bottom
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.75f, 0.8f, 1.0f));  // Pink
                                                                                            // color
                    ImGui::Text("%d", quantity);
                    ImGui::PopStyleColor();

                    // Reset font scale to normal
                    ImGui::SetWindowFontScale(1.0f);
                }
            }
            else
            {
                ImVec2 itemButtonSize(SLOT_SIZE - 16, SLOT_SIZE - 16);
                ImVec2 itemButtonPos(buttonCenter.x - itemButtonSize.x / 2.0f,
                                     buttonCenter.y - itemButtonSize.y / 2.0f);
                ImGui::SetCursorScreenPos(itemButtonPos);
                ImGui::Button(item.getName().c_str(), itemButtonSize);

                // Only show item details for non-empty slots
                if (!isEmptySlot)
                {
                    // Temporarily increase font size for item details
                    ImGui::SetWindowFontScale(1.1f);

                    // Rarity label (top-left with padding) - Different colors for different
                    // rarities
                    char r = item.getRarity();
                    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 12, buttonPos.y + 10));  // More
                                                                                            // padding
                                                                                            // from
                                                                                            // corner

                    // Set color based on rarity
                    ImVec4 rarityColor;
                    switch (r)
                    {
                        case 'S':
                            rarityColor = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
                            break;  // Light blue for S
                        case 'A':
                            rarityColor = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
                            break;  // Light green for A
                        case 'B':
                            rarityColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
                            break;  // Orange for B
                        case 'C':
                            rarityColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f);
                            break;  // Light red for C
                        case 'D':
                            rarityColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                            break;  // Light grey for D
                        case 'E':
                            rarityColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                            break;  // Darker grey for E
                        default:
                            rarityColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                            break;  // White for others
                    }

                    ImGui::PushStyleColor(ImGuiCol_Text, rarityColor);
                    ImGui::Text("%c", r);
                    ImGui::PopStyleColor();

                    // Price label (bottom-left with padding) - Yellow color with $
                    ImGui::SetCursorScreenPos(
                        ImVec2(buttonPos.x + 12, buttonPos.y + SLOT_SIZE - 26));  // More padding
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));  // Yellow
                                                                                           // color
                    ImGui::Text("$%d", price);  // Added $ before price
                    ImGui::PopStyleColor();

                    // Quantity label (bottom-right with padding) - Pink color
                    std::string quantityText     = std::to_string(quantity);
                    ImVec2      quantityTextSize = ImGui::CalcTextSize(quantityText.c_str());
                    ImGui::SetCursorScreenPos(
                        ImVec2(buttonPos.x + SLOT_SIZE - quantityTextSize.x - 17,  // More padding
                                                                                   // from edge
                               buttonPos.y + SLOT_SIZE - 26));  // More padding from bottom
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.75f, 0.8f, 1.0f));  // Pink
                                                                                            // color
                    ImGui::Text("%d", quantity);
                    ImGui::PopStyleColor();

                    // Reset font scale to normal
                    ImGui::SetWindowFontScale(1.0f);
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
    ImGui::PopStyleVar(6);
    ImGui::PopStyleColor(6);

    // Clear hovered item when nothing is hovered
    if (!ImGui::IsAnyItemHovered() || !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
    {
        m_hoveredItem = Item();  // Reset to empty item
    }
}

ImVec4 ShopMenu::GetItemColor(const std::string& itemName, const std::string& itemType)
{
    // m_equipment type colors
    if (itemType == "Potion")
        return ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
    else if (itemType == "Weapon")
        return ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    else if (itemType == "HeadArmor" || itemType == "BodyArmor" || itemType == "FootArmor")
        return ImVec4(0.2f, 0.6f, 0.8f, 1.0f);
    else if (itemType == "Pendant")
        return ImVec4(0.8f, 0.6f, 0.2f, 1.0f);

    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

ImVec4 ShopMenu::GetRarityColor(char rarity) const
{
    switch (rarity)
    {
        case 'S':
            return ImVec4(0.5f, 0.5f, 1.0f, 1.0f);  // Light blue for S
        case 'A':
            return ImVec4(0.5f, 1.0f, 0.5f, 1.0f);  // Light green for A
        case 'B':
            return ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange for B
        case 'C':
            return ImVec4(1.0f, 0.5f, 0.5f, 1.0f);  // Light red for C
        case 'D':
            return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);  // Light grey for D
        case 'E':
            return ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // Darker grey for E
        default:
            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // White for others
    }
}
