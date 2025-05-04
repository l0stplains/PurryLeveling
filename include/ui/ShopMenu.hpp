#pragma once

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "core/GameContext.hpp"

#include "imgui-SFML.h"
#include "imgui.h"
#include "inventory/Backpack.hpp"
#include "items/Item.hpp"
#include "shop/Shop.hpp"

class ShopMenu
{
public:
    ShopMenu(GameContext&);

    void Render();
    bool IsModalOpen() const;

private:
    static constexpr int SLOT_SIZE      = 138;
    static constexpr int SLOT_PADDING   = 12;
    static constexpr int MAX_STACK_SIZE = 64;
    static constexpr int GRID_WIDTH     = 4;
    int                  GRID_HEIGHT;

    GameContext& m_gameContext;
    Backpack&    m_backpack;
    Shop&        m_shop;
    int          m_playerID;

    // Member variables for filters and selling mode
    std::vector<std::string> m_activeCategories;
    std::string              m_currentSort;  // "Rarity", "Price", "Stock", or empty
    std::string              m_sortState;    // "un", "desc", "asc"
    bool m_isSellingMode;  // true = selling from backpack, false = buying from shop

    // Stock vector as member variable
    std::vector<Shop::StockEntry> m_stock;

    // Description for hovered item
    Item m_hoveredItem;

    // Modal state variables
    bool m_showModal;
    Item m_selectedItem;
    int  m_selectedPrice;
    int  m_selectedQuantity;
    int  m_selectedQtyInStock;
    int  m_purchaseQty;
    int  m_totalPrice;

    // Error message state
    std::string m_errorMessage;
    bool        m_showError;

    // Methods for rendering UI components
    void RenderGrid();
    void RenderEquipment(float startX, float startY);
    void RenderGoldDisplay();
    void RenderControlPanel();  // All controls in one group
    void RenderItemDescription();
    void RenderModal();

    void RefreshStock();  // Helper to refresh stock when changing mode/filters/sort

    ImVec4 GetItemColor(const std::string& itemname, const std::string& itemtype);
    ImVec4 GetRarityColor(char rarity) const;

    // Error handling methods
    void ShowError(const std::string& message);
    void ClearError();
};