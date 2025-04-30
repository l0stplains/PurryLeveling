#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"
#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
#include "items/Item.hpp"

class InventorySystem
{
    // A simple POD-safe payload for ImGui drag & drop:
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
    static constexpr int GRID_WIDTH     = 8;  // Backpack is 8 columns (horizontal)
    static constexpr int GRID_HEIGHT    = 4;  // Backpack is 4 rows (vertical)
    static constexpr int SLOT_SIZE      = 60;
    static constexpr int SLOT_PADDING   = 4;
    static constexpr int EQUIP_SLOTS    = 5;   // 5 equipment slots
    static constexpr int MAX_STACK_SIZE = 64;  // Maximum stack size for items

    InventorySystem();

    void render();

private:
    Backpack                              backpack;
    Equipment                             equipment;
    std::unique_ptr<std::pair<Item, int>> draggedItem;
    bool                                  isDragging              = false;
    int                                   dragSourceX             = -1;
    int                                   dragSourceY             = -1;
    bool                                  isDraggingFromEquipment = false;
    std::string                           equipmentSlotType;

    void renderBackpack(float startX, float startY);

    void renderEquipment(float startX, float startY);

    // Helper function to get color for item visualization
    ImVec4 getItemColor(const std::string& itemName, const std::string& itemType);

    // Add some random items to inventory for testing
    void addRandomItems();

    // Clear all inventory slots
    void clearInventory();
};