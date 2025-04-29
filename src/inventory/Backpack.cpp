#include "../include/inventory/Backpack.hpp"

#include <utility>  // for std::pair

#include "../include/exception/Exception.hpp"

Backpack::Backpack(const std::vector<std::pair<Item, int>>& initial, int rows, int cols)
    : backpackRows(rows), backpackCols(cols)
{
    // Fill the 2D grid with default empty slots
    backpack.assign(backpackRows, std::vector<std::pair<Item, int>>(backpackCols, {Item(), 0}));

    // Copy up to rows*cols elements from the flat vector
    const int maxCount = backpackRows * backpackCols;
    const int count    = std::min(int(initial.size()), maxCount);
    for (int idx = 0; idx < count; ++idx)
    {
        int i          = idx / backpackCols;
        int j          = idx % backpackCols;
        backpack[i][j] = initial[idx];
    }
}

Backpack::~Backpack() {}

void Backpack::validateCoordinates(int x, int y) const
{
    if (x < 0 || x >= backpackRows || y < 0 || y >= backpackCols)
    {
        throw OutOfBoundsException();
    }
}

std::pair<Item, int> Backpack::rawAt(int x, int y) const
{
    validateCoordinates(x, y);
    return backpack[x][y];
}

Item Backpack::getItemAtTile(int x, int y) const
{
    validateCoordinates(x, y);
    const auto& cell = backpack[x][y];
    if (cell.first.isNull())
    {
        throw EmptyCellException();
    }
    return cell.first;
}

int Backpack::getQuantityAtTile(int x, int y) const
{
    validateCoordinates(x, y);
    const auto& cell = backpack[x][y];
    if (cell.first.isNull())
    {
        throw EmptyCellException();
    }
    return cell.second;
}

// Helper method to find a tile with the same item that can accept more quantity
std::pair<bool, std::pair<int, int>> Backpack::findItemTile(const Item& item) const
{
    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            const auto& cell = backpack[i][j];
            if (!cell.first.isNull() && cell.first == item && cell.second < maxStackSize)
            {
                return {true, {i, j}};
            }
        }
    }
    return {false, {-1, -1}};  // Not found
}

// Helper method to find an empty tile
std::pair<bool, std::pair<int, int>> Backpack::findEmptyTile() const
{
    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            const auto& cell = backpack[i][j];
            if (cell.first.isNull())
            {
                return {true, {i, j}};
            }
        }
    }
    return {false, {-1, -1}};  // Not found
}

// Fixed implementation for addItemAtTile to prevent replacing different items
void Backpack::addItemAtTile(int x, int y, const Item& item, int quantity)
{
    validateCoordinates(x, y);

    if (quantity <= 0)
    {
        throw InvalidQuantityException();
    }

    if (item.isNull())
    {
        throw QuantityRequiresValidItemException();
    }

    if (quantity > maxStackSize)
    {
        throw InvalidQuantityException();
    }

    try
    {
        // Check if there's already an item
        Item currentItem     = getItemAtTile(x, y);
        int  currentQuantity = getQuantityAtTile(x, y);

        if (currentItem == item)
        {
            // Same item type - try to stack
            int totalQuantity = currentQuantity + quantity;
            if (totalQuantity > maxStackSize)
            {
                throw InvalidQuantityException();
            }

            // Update the quantity
            backpack[x][y].second = totalQuantity;
        }
        else
        {
            // Different item - throw an exception instead of replacing
            throw ItemSlotOccupiedException("Cannot add a different item to an occupied slot");
        }
    }
    catch (const EmptyCellException&)
    {
        // Empty slot - add the new item
        backpack[x][y] = std::make_pair(item, quantity);
    }
}

// Implementation for addItem with desired flow
void Backpack::addItem(const Item& item, int quantity)
{
    if (quantity <= 0)
    {
        return;  // Nothing to add
    }

    if (item.isNull())
    {
        throw QuantityRequiresValidItemException();
    }

    int remainingQuantity = quantity;

    // First, try to fill existing stacks of the same item
    while (remainingQuantity > 0)
    {
        auto [found, location] = findItemTile(item);
        if (!found)
        {
            break;  // No existing stacks with space left
        }

        int x = location.first;
        int y = location.second;

        try
        {
            int currentQuantity = getQuantityAtTile(x, y);
            int canAdd          = std::min(maxStackSize - currentQuantity, remainingQuantity);

            addItemAtTile(x, y, item, canAdd);
            remainingQuantity -= canAdd;
        }
        catch (const std::exception&)
        {
            // If there's any error, try the next stack
            continue;
        }
    }

    // If we still have items left, use empty slots
    while (remainingQuantity > 0)
    {
        auto [found, location] = findEmptyTile();
        if (!found)
        {
            throw BackpackOvercapacityException();
        }

        int x      = location.first;
        int y      = location.second;
        int canAdd = std::min(maxStackSize, remainingQuantity);

        addItemAtTile(x, y, item, canAdd);
        remainingQuantity -= canAdd;
    }
}

// Updated takeItemAtTile with quantity parameter
Item Backpack::takeItemAtTile(int x, int y, int quantity)
{
    validateCoordinates(x, y);

    if (quantity <= 0)
    {
        throw InvalidQuantityException();
    }

    Item item            = getItemAtTile(x, y);
    int  currentQuantity = getQuantityAtTile(x, y);

    if (quantity > currentQuantity)
    {
        throw InsufficientQuantityException();
    }

    if (quantity == currentQuantity)
    {
        // Taking all items
        backpack[x][y] = std::make_pair(Item(), 0);
    }
    else
    {
        // Taking some items
        backpack[x][y].second = currentQuantity - quantity;
    }

    return item;
}

// Implementation for takeItem with desired flow
Item Backpack::takeItem(const Item& item, int quantity)
{
    if (quantity <= 0)
    {
        throw InvalidQuantityException();
    }

    // Count total available items
    int totalAvailable = 0;
    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            try
            {
                Item currentItem = getItemAtTile(i, j);
                if (currentItem == item)
                {
                    totalAvailable += getQuantityAtTile(i, j);
                }
            }
            catch (const EmptyCellException&)
            {
                // Skip empty cells
                continue;
            }
        }
    }

    if (totalAvailable < quantity)
    {
        throw InsufficientQuantityException();
    }

    Item takenItem       = item;  // Store the item type to return
    int  remainingToTake = quantity;

    // First take from non-full stacks to minimize fragmentation
    for (int i = 0; i < backpackRows && remainingToTake > 0; ++i)
    {
        for (int j = 0; j < backpackCols && remainingToTake > 0; ++j)
        {
            try
            {
                Item currentItem = getItemAtTile(i, j);
                if (currentItem == item)
                {
                    int currentQuantity = getQuantityAtTile(i, j);
                    if (currentQuantity < maxStackSize)
                    {
                        // Take from this stack
                        int toTake = std::min(remainingToTake, currentQuantity);
                        takeItemAtTile(i, j, toTake);
                        remainingToTake -= toTake;
                    }
                }
            }
            catch (const std::exception&)
            {
                // Skip problematic cells
                continue;
            }
        }
    }

    // If we still need items, take from any stack
    for (int i = 0; i < backpackRows && remainingToTake > 0; ++i)
    {
        for (int j = 0; j < backpackCols && remainingToTake > 0; ++j)
        {
            try
            {
                Item currentItem = getItemAtTile(i, j);
                if (currentItem == item)
                {
                    int currentQuantity = getQuantityAtTile(i, j);
                    int toTake          = std::min(remainingToTake, currentQuantity);
                    takeItemAtTile(i, j, toTake);
                    remainingToTake -= toTake;
                }
            }
            catch (const std::exception&)
            {
                // Skip problematic cells
                continue;
            }
        }
    }

    return takenItem;
}

// Implementation for moveItem
void Backpack::moveItem(int fromX, int fromY, int toX, int toY)
{
    validateCoordinates(fromX, fromY);
    validateCoordinates(toX, toY);

    // Check if source has an item
    try
    {
        Item sourceItem     = getItemAtTile(fromX, fromY);
        int  sourceQuantity = getQuantityAtTile(fromX, fromY);

        try
        {
            // Try to get destination item
            Item destItem     = getItemAtTile(toX, toY);
            int  destQuantity = getQuantityAtTile(toX, toY);

            if (sourceItem == destItem)
            {
                // Same items, stack them
                int total = sourceQuantity + destQuantity;
                if (total > maxStackSize)
                {
                    int canMove = maxStackSize - destQuantity;
                    // Update destination
                    backpack[toX][toY].second = maxStackSize;
                    // Update source
                    backpack[fromX][fromY].second = sourceQuantity - canMove;
                }
                else
                {
                    // Update destination
                    backpack[toX][toY].second = total;
                    // Clear source
                    backpack[fromX][fromY] = std::make_pair(Item(), 0);
                }
            }
            else
            {
                // Different items, swap them
                std::swap(backpack[fromX][fromY], backpack[toX][toY]);
            }
        }
        catch (const EmptyCellException&)
        {
            // Destination is empty
            // Move item to empty destination
            backpack[toX][toY] = std::make_pair(sourceItem, sourceQuantity);
            // Clear source
            backpack[fromX][fromY] = std::make_pair(Item(), 0);
        }
    }
    catch (const EmptyCellException&)
    {
        throw;  // Rethrow if source is empty
    }
}

std::vector<std::pair<Item, int>> Backpack::filterItemsByType(const std::string& type) const
{
    std::vector<std::pair<Item, int>> result;

    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            const auto& slot     = backpack[i][j];
            const Item& item     = slot.first;
            int         quantity = slot.second;

            // Assume Item has a method getType() returning its type string
            if (quantity > 0 && item.getType() == type)
            {
                result.push_back(slot);
            }
        }
    }
    return result;
}