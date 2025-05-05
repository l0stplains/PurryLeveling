#pragma once

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "core/GameContext.hpp"

#include "ui/Color.hpp"

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
    InventoryMenu(GameContext& gameContext);
    void Render();

private:
    GameContext& m_gameContext;
    Backpack&    m_backpack;
    Equipment&   m_equipment;

    std::string m_hoveredDescription;

    // Drag & drop support
    bool                                  m_isDragging;
    int                                   m_dragSourceX;
    int                                   m_dragSourceY;
    bool                                  m_isDraggingFromEquipment;
    std::string                           m_equipmentSlotType;
    std::unique_ptr<std::pair<Item, int>> m_draggedItem;

    // Error handling
    bool        m_showErrorPopup;
    std::string m_errorMessage;

    // Component rendering methods
    void RenderBackpack(float startX, float startY);
    void RenderEquipment(float startX, float startY);
    void RenderDescriptionBox(float startX, float startY);
    void RenderErrorPopups();
};