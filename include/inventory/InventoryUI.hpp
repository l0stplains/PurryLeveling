// InventoryUI.hpp
#pragma once

#include <imgui.h>

#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"

class InventoryUI
{
public:
    InventoryUI(Backpack& backpack, Equipment& equipment);
    void render();

private:
    Backpack&  backpack;
    Equipment& equipment;

    enum class PayloadType : int
    {
        Backpack  = 0,
        Equipment = 1
    };
    struct DragPayload
    {
        PayloadType type;
        int         fromRow;
        int         fromCol;
        int         equipSlot;
    };

    // UI grid dimensions
    static constexpr int invRows   = 4;
    static constexpr int invCols   = 8;
    static constexpr int equipRows = 3;
    static constexpr int equipCols = 2;
    static constexpr int SLOT_SIZE = 60;
    static constexpr int SLOT_PAD  = 4;

    bool isDragging      = false;
    int  dragSourceIndex = -1;

    const std::string slotTypeName(int equipSlot) const;
    void              handleDropOnBackpack(int toRow, int toCol, const DragPayload& payload);
    void              handleDropOnEquipment(int toSlot, const DragPayload& payload);
};
