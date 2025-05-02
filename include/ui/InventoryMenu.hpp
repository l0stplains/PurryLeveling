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
#include "inventory/Equipment.hpp"
#include "items/Item.hpp"

class InventoryMenu
{
    class DragDropPayload
    {
    public:
        bool fromEquipment;
        int  sourceX;
        int  sourceY;
        char slotType[16];

        // constructor for easy initialization
        DragDropPayload(bool fromEquip, int x, int y, const std::string& type);
    };

public:
    InventoryMenu(GameContext&);

    void Render();

private:
    static constexpr int SLOT_SIZE      = 92;
    static constexpr int SLOT_PADDING   = 12;
    static constexpr int MAX_STACK_SIZE = 64;
    static constexpr int EQUIP_SLOTS    = 5;
    static constexpr int GRID_WIDTH     = 8;
    static constexpr int GRID_HEIGHT    = 4;

    GameContext&                          m_gameContext;
    Backpack&                             m_backpack;
    Equipment&                            m_equipment;
    std::unique_ptr<std::pair<Item, int>> m_draggedItem;
    bool                                  m_isDragging              = false;
    int                                   m_dragSourceX             = -1;
    int                                   m_dragSourceY             = -1;
    bool                                  m_isDraggingFromEquipment = false;
    std::string                           m_equipmentSlotType;

    void RenderBackpack(float startX, float startY);

    void RenderEquipment(float startX, float startY);

    ImVec4 GetItemColor(const std::string& itemname, const std::string& itemtype);
};