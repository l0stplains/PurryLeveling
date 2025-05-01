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

private:
    static constexpr int SLOT_SIZE      = 92;
    static constexpr int SLOT_PADDING   = 12;
    static constexpr int MAX_STACK_SIZE = 64;
    static constexpr int GRID_WIDTH     = 6;
    static constexpr int GRID_HEIGHT    = 10;
    // int GRID_HEIGHT = 5;

    GameContext& m_gameContext;
    Backpack&    m_backpack;
    // Shop&                              m_shop;
    int m_playerID;

    void RenderGrid(float startX, float startY);

    void RenderEquipment(float startX, float startY);

    ImVec4 GetItemColor(const std::string& itemname, const std::string& itemtype);
};