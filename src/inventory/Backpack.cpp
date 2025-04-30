#include "inventory/Backpack.hpp"

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

std::vector<std::pair<int, int>> Backpack::findItemTile(const Item& item) const
{
    std::vector<std::pair<int, int>> result;

    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            const auto& cell = backpack[i][j];
            if (!cell.first.isNull() && cell.first == item)
            {
                result.push_back({i, j});
            }
        }
    }

    return result;  // Return all found tiles
}

std::vector<std::pair<int, int>> Backpack::findEmptyTile() const
{
    std::vector<std::pair<int, int>> result;

    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            const auto& cell = backpack[i][j];
            if (cell.first.isNull())
            {
                result.push_back({i, j});
            }
        }
    }

    return result;  // Return all found empty tiles
}

void Backpack::addItemAtTile(int x, int y, const Item& item, int quantity)
{
    validateCoordinates(x, y);

    if (quantity <= 0 || quantity > maxStackSize)
    {
        throw InvalidQuantityException();
    }

    if (item.isNull())
    {
        throw QuantityRequiresValidItemException();
    }

    // Check if there's already an item
    Item currentItem;
    int  currentQuantity = 0;
    bool isEmpty         = false;

    // Use direct check for empty cell instead of try-catch
    const auto& cell = backpack[x][y];
    isEmpty          = cell.first.isNull();

    if (!isEmpty)
    {
        currentItem     = cell.first;
        currentQuantity = cell.second;

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
            throw ItemSlotOccupiedException();
        }
    }
    else
    {
        // Empty slot - add the new item
        backpack[x][y] = std::make_pair(item, quantity);
    }
}

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

    // Pre-calculate available space
    int availableSpace = 0;

    // Find existing tiles with the same item
    std::vector<std::pair<int, int>> itemTiles = findItemTile(item);
    for (const auto& [x, y] : itemTiles)
    {
        int currentQuantity = backpack[x][y].second;
        if (currentQuantity < maxStackSize)
        {
            availableSpace += (maxStackSize - currentQuantity);
        }
    }

    // Find empty tiles
    std::vector<std::pair<int, int>> emptyTiles = findEmptyTile();
    availableSpace += emptyTiles.size() * maxStackSize;

    // Check if we have enough space
    if (availableSpace < quantity)
    {
        throw BackpackOvercapacityException();
    }

    // Now proceed with adding items
    int remainingQuantity = quantity;

    // First, try to fill existing stacks of the same item
    for (const auto& [x, y] : itemTiles)
    {
        if (remainingQuantity <= 0)
            break;

        int currentQuantity = backpack[x][y].second;
        int canAdd          = std::min(maxStackSize - currentQuantity, remainingQuantity);

        // Update the quantity at this tile
        backpack[x][y].second += canAdd;
        remainingQuantity -= canAdd;
    }

    // If we still have items left, use empty slots
    for (const auto& [x, y] : emptyTiles)
    {
        if (remainingQuantity <= 0)
            break;

        int canAdd = std::min(maxStackSize, remainingQuantity);

        // Add new item at this empty tile
        backpack[x][y] = std::make_pair(item, canAdd);
        remainingQuantity -= canAdd;
    }
}

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

Item Backpack::takeItem(const Item& item, int quantity)
{
    if (quantity <= 0)
    {
        throw InvalidQuantityException();
    }

    // Get all tiles containing the item using findItemTile
    std::vector<std::pair<int, int>> itemTileCoords = findItemTile(item);

    // Count total available items
    int totalAvailable = 0;
    for (const auto& [x, y] : itemTileCoords)
    {
        totalAvailable += backpack[x][y].second;
    }

    // Check if we have enough items
    if (totalAvailable < quantity)
    {
        throw InsufficientQuantityException();
    }

    Item takenItem       = item;  // Store the item type to return
    int  remainingToTake = quantity;

    // First take from partially filled stacks to minimize fragmentation
    // Filter for partially filled stacks
    std::vector<std::pair<int, int>> partiallyFilledTiles;
    for (const auto& [x, y] : itemTileCoords)
    {
        if (backpack[x][y].second < maxStackSize)
        {
            partiallyFilledTiles.push_back({x, y});
        }
    }

    // Take from partially filled stacks first
    for (const auto& [x, y] : partiallyFilledTiles)
    {
        if (remainingToTake <= 0)
            break;

        int currentQuantity = backpack[x][y].second;
        int toTake          = std::min(remainingToTake, currentQuantity);

        // Update this tile
        if (toTake == currentQuantity)
        {
            // Taking all items
            backpack[x][y] = std::make_pair(Item(), 0);
        }
        else
        {
            // Taking some items
            backpack[x][y].second = currentQuantity - toTake;
        }

        remainingToTake -= toTake;
    }

    // If we still need items, take from any remaining stacks
    for (const auto& [x, y] : itemTileCoords)
    {
        if (remainingToTake <= 0)
            break;

        // Skip if this was a partially filled tile we already processed
        // or if it's now empty
        if (backpack[x][y].first.isNull())
            continue;

        int currentQuantity = backpack[x][y].second;
        int toTake          = std::min(remainingToTake, currentQuantity);

        // Update this tile
        if (toTake == currentQuantity)
        {
            // Taking all items
            backpack[x][y] = std::make_pair(Item(), 0);
        }
        else
        {
            // Taking some items
            backpack[x][y].second = currentQuantity - toTake;
        }

        remainingToTake -= toTake;
    }

    return takenItem;
}

void Backpack::moveItem(int fromX, int fromY, int toX, int toY)
{
    validateCoordinates(fromX, fromY);
    validateCoordinates(toX, toY);

    // Check if source has an item
    Item sourceItem     = getItemAtTile(fromX, fromY);
    int  sourceQuantity = getQuantityAtTile(fromX, fromY);

    bool destIsEmpty = false;

    // Check if destination is empty (without try-catch)
    const auto& destCell = backpack[toX][toY];
    destIsEmpty          = destCell.first.isNull();

    if (!destIsEmpty)
    {
        // Destination has an item
        Item destItem     = destCell.first;
        int  destQuantity = destCell.second;

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
    else
    {
        // Destination is empty
        // Move item to empty destination
        backpack[toX][toY] = std::make_pair(sourceItem, sourceQuantity);
        // Clear source
        backpack[fromX][fromY] = std::make_pair(Item(), 0);
    }
}

std::vector<std::reference_wrapper<std::pair<Item, int>>> Backpack::filterItemsByType(
    const std::string& type)
{
    std::vector<std::reference_wrapper<std::pair<Item, int>>> result;

    for (int i = 0; i < backpackRows; ++i)
    {
        for (int j = 0; j < backpackCols; ++j)
        {
            auto&       slot     = backpack[i][j];  // Remove const here
            const Item& item     = slot.first;
            int         quantity = slot.second;

            if (quantity > 0 && item.getType() == type)
            {
                result.emplace_back(slot);
            }
        }
    }
    return result;
}