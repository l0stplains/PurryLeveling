#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"

// Structure to represent an item in inventory
struct InventoryItem
{
    std::string name;
    int         count;
    sf::Texture texture;
    bool        empty() const { return count <= 0 || name.empty(); }

    InventoryItem() : name(""), count(0) {}

    InventoryItem(const std::string& n, int c) : name(n), count(c)
    {
        // In a real implementation, you would load textures based on item name
        texture.loadFromFile("resources/textures/props/empty.png");
    }
};

// Inventory slot class
class InventorySlot
{
public:
    InventorySlot() : item() {}

    void addItem(const InventoryItem& newItem)
    {
        if (item.empty())
        {
            item = newItem;
        }
        else if (item.name == newItem.name)
        {
            item.count += newItem.count;
        }
    }

    bool canAddItem(const InventoryItem& newItem) const
    {
        return item.empty() || (item.name == newItem.name && item.count + newItem.count <= 64);
    }

    int addItemCount(const std::string& itemName, int count)
    {
        if (item.empty())
        {
            item = InventoryItem(itemName, count);
            return 0;
        }
        else if (item.name == itemName)
        {
            int spaceLeft   = 64 - item.count;
            int amountToAdd = std::min(count, spaceLeft);
            item.count += amountToAdd;
            return count - amountToAdd;
        }
        return count;
    }

    void removeItem() { item = InventoryItem(); }

    void removeHalf()
    {
        if (!item.empty())
        {
            item.count = item.count / 2;
            if (item.count <= 0)
            {
                item = InventoryItem();
            }
        }
    }

    InventoryItem item;
};

class InventorySystem
{
public:
    static constexpr int GRID_WIDTH   = 8;
    static constexpr int GRID_HEIGHT  = 6;
    static constexpr int SLOT_SIZE    = 60;
    static constexpr int SLOT_PADDING = 4;

    InventorySystem()
    {
        slots.resize(GRID_WIDTH * GRID_HEIGHT);
        draggedItem = std::make_unique<InventoryItem>();

        // Initialize some sample items
        slots[0].addItem(InventoryItem("Diamond", 5));
        slots[1].addItem(InventoryItem("Iron", 45));
        slots[9].addItem(InventoryItem("Gold", 12));
        slots[16].addItem(InventoryItem("Diamond", 64));
        slots[24].addItem(InventoryItem("Stone", 64));
        slots[32].addItem(InventoryItem("Wood", 32));
    }

    void render()
    {
        isDragging = false;

        // Handle dragged item rendering
        ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_None);

        // Set window size to fit the inventory grid
        ImGui::SetWindowSize(ImVec2(GRID_WIDTH * SLOT_SIZE + SLOT_PADDING * 2,
                                    GRID_HEIGHT * SLOT_SIZE + SLOT_PADDING * 2 + 40));

        // Calculate positions for grid
        ImVec2 curPos = ImGui::GetCursorScreenPos();
        float  startX = curPos.x + SLOT_PADDING;
        float  startY = curPos.y + SLOT_PADDING;

        // Render inventory grid
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            for (int x = 0; x < GRID_WIDTH; x++)
            {
                int            index = y * GRID_WIDTH + x;
                InventorySlot& slot  = slots[index];

                // Set position for this slot
                ImGui::SetCursorScreenPos(ImVec2(startX + x * SLOT_SIZE, startY + y * SLOT_SIZE));

                // Create a selectable area for the slot
                ImGui::PushID(index);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

                if (ImGui::Button("", ImVec2(SLOT_SIZE - 8, SLOT_SIZE - 8)))
                {
                    // Handle clicks on slot
                    if (!slot.item.empty())
                    {
                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            // Double-click handling (could be used for equipping or using items)
                            std::cout << "Double-clicked on " << slot.item.name << std::endl;
                        }
                    }
                }

                // Drag source - only create for non-empty slots
                if (!slot.item.empty() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    isDragging      = true;
                    dragSourceIndex = index;

                    // Prepare the payload
                    ImGui::SetDragDropPayload("INVENTORY_ITEM", &index, sizeof(int));

                    // Show item being dragged
                    ImGui::Text("%s x%d", slot.item.name.c_str(), slot.item.count);

                    ImGui::EndDragDropSource();
                }

                // Drop target - always create for all slots
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INVENTORY_"
                                                                                   "ITEM"))
                    {
                        int            sourceIdx  = *(const int*)payload->Data;
                        InventorySlot& sourceSlot = slots[sourceIdx];

                        // Handle different drag behaviors based on key modifiers
                        if (ImGui::GetIO().KeyShift)
                        {
                            // Shift-click: Add one item
                            handleItemTransfer(sourceSlot, slot, 1);
                        }
                        else if (ImGui::GetIO().KeyCtrl)
                        {
                            // Ctrl-click: Add half of stack
                            int halfAmount = sourceSlot.item.count / 2;
                            if (halfAmount > 0)
                            {
                                handleItemTransfer(sourceSlot, slot, halfAmount);
                            }
                        }
                        else
                        {
                            // Normal click: Move entire stack
                            handleItemTransfer(sourceSlot, slot, sourceSlot.item.count);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                // Render item in slot if it exists
                if (!slot.item.empty())
                {
                    ImGui::SetCursorScreenPos(
                        ImVec2(startX + x * SLOT_SIZE + 4, startY + y * SLOT_SIZE + 4));

                    // In a real implementation, you'd render the item texture here
                    ImGui::PushStyleColor(ImGuiCol_Button, getItemColor(slot.item.name));
                    ImGui::Button(slot.item.name.c_str(), ImVec2(SLOT_SIZE - 16, SLOT_SIZE - 16));
                    ImGui::PopStyleColor();

                    // Show stack count for items with count > 1
                    if (slot.item.count > 1)
                    {
                        std::string countText = std::to_string(slot.item.count);
                        ImVec2      textSize  = ImGui::CalcTextSize(countText.c_str());
                        ImGui::SetCursorScreenPos(
                            ImVec2(startX + (x + 1) * SLOT_SIZE - textSize.x - 10,
                                   startY + (y + 1) * SLOT_SIZE - textSize.y - 8));
                        ImGui::Text("%d", slot.item.count);
                    }
                }

                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }

        // Add some control buttons
        ImGui::SetCursorScreenPos(ImVec2(startX, startY + GRID_HEIGHT * SLOT_SIZE + 10));
        if (ImGui::Button("Add Random Items"))
        {
            addRandomItems();
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear All"))
        {
            clearInventory();
        }

        ImGui::End();
    }

private:
    std::vector<InventorySlot>     slots;
    std::unique_ptr<InventoryItem> draggedItem;
    bool                           isDragging      = false;
    int                            dragSourceIndex = -1;

    // Handle item transfer logic between slots
    void handleItemTransfer(InventorySlot& source, InventorySlot& target, int amount)
    {
        // Early return if trying to move to the same slot
        if (&source == &target)
            return;

        // Cap the amount we're transferring to what's available
        amount = std::min(amount, source.item.count);
        if (amount <= 0)
            return;

        // Logic for transferring items
        if (target.item.empty())
        {
            // Target slot is empty, create a new stack
            target.item = InventoryItem(source.item.name, amount);
            source.item.count -= amount;
        }
        else if (target.item.name == source.item.name)
        {
            // Same item type, try to stack
            int spaceInTarget = 64 - target.item.count;
            int amountToAdd   = std::min(amount, spaceInTarget);

            target.item.count += amountToAdd;
            source.item.count -= amountToAdd;
        }

        // Clean up empty source slot
        if (source.item.count <= 0)
        {
            source.removeItem();
        }
    }

    // Helper function to get color for item visualization
    ImVec4 getItemColor(const std::string& itemName)
    {
        if (itemName == "Diamond")
            return ImVec4(0.0f, 0.8f, 1.0f, 1.0f);
        if (itemName == "Iron")
            return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        if (itemName == "Gold")
            return ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
        if (itemName == "Stone")
            return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        if (itemName == "Wood")
            return ImVec4(0.6f, 0.4f, 0.2f, 1.0f);
        return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Add some random items to inventory for testing
    void addRandomItems()
    {
        const std::vector<std::string> itemTypes = {"Diamond", "Iron", "Gold", "Stone", "Wood"};

        for (int i = 0; i < 10; i++)
        {
            int         randomSlot = rand() % slots.size();
            std::string itemType   = itemTypes[rand() % itemTypes.size()];
            int         itemCount  = 1 + rand() % 64;

            // Find an empty slot or matching item type
            bool added = false;
            for (int j = 0; j < slots.size(); j++)
            {
                int slotIndex = (randomSlot + j) % slots.size();

                if (slots[slotIndex].item.empty() ||
                    (slots[slotIndex].item.name == itemType && slots[slotIndex].item.count < 64))
                {
                    int remaining = slots[slotIndex].addItemCount(itemType, itemCount);
                    if (remaining == 0)
                    {
                        added = true;
                        break;
                    }

                    // If there's remaining items, update count and continue trying
                    itemCount = remaining;
                }
            }
        }
    }

    // Clear all inventory slots
    void clearInventory()
    {
        for (auto& slot : slots)
        {
            slot.removeItem();
        }
    }
};

int main()
{
    // Create SFML window
    const unsigned int WINDOW_WIDTH  = 1280;  // Use consistent window size
    const unsigned int WINDOW_HEIGHT = 720;
    sf::RenderWindow   window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Coba coba inventory");
    window.setFramerateLimit(60);

    // Initialize ImGui
    ImGui::SFML::Init(window);

    // Create inventory system
    InventorySystem inventory;

    // Main loop
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        // --- Event Handling ---
        while (auto eventOpt = window.pollEvent())
        {
            sf::Event event = *eventOpt;  // Dereference the optional
            ImGui::SFML::ProcessEvent(window, event);

            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());

        // Render inventory
        inventory.render();

        // Clear window and draw ImGui
        window.clear(sf::Color(64, 64, 64));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}