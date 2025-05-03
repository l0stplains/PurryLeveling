#include "inventory/Equipment.hpp"

Equipment::Equipment()
{
    this->weapon    = Item();
    this->headArmor = Item();
    this->bodyArmor = Item();
    this->footArmor = Item();
    this->pendant   = Item();
}

void Equipment::setEquipmentData(ItemManager&                                 itemManager,
                                 const std::vector<std::vector<std::string>>& equipmentData)
{
    for (int i = 0; i < equipmentData.size(); ++i)
    {
        const auto& slot   = equipmentData[i][0];
        const auto& itemID = equipmentData[i][1];

        if (slot == "WEAPON")
        {
            weapon = itemManager.getItem(itemID);
        }
        else if (slot == "ARMOR_HEAD")
        {
            headArmor = itemManager.getItem(itemID);
        }
        else if (slot == "ARMOR_BODY")
        {
            bodyArmor = itemManager.getItem(itemID);
        }
        else if (slot == "ARMOR_FOOT")
        {
            footArmor = itemManager.getItem(itemID);
        }
        else if (slot == "PENDANT")
        {
            pendant = itemManager.getItem(itemID);
        }
    }
}

Item Equipment::getWeapon() const
{
    return weapon;
}

Item Equipment::getHeadArmor() const
{
    return headArmor;
}

Item Equipment::getBodyArmor() const
{
    return bodyArmor;
}

Item Equipment::getFootArmor() const
{
    return footArmor;
}

Item Equipment::getPendant() const
{
    return pendant;
}

void Equipment::setWeapon(const Item& newWeapon)
{
    if (!newWeapon.isNull() && newWeapon.getType() != "Weapon")
    {
        throw InvalidEquipmentTypeException("Weapon slot requires an item of type 'Weapon'");
    }
    weapon = newWeapon;
}

void Equipment::setHeadArmor(const Item& newHeadArmor)
{
    if (!newHeadArmor.isNull() && newHeadArmor.getType() != "HeadArmor")
    {
        throw InvalidEquipmentTypeException("Head armor slot requires an item of type 'HeadArmor'");
    }
    headArmor = newHeadArmor;
}

void Equipment::setBodyArmor(const Item& newBodyArmor)
{
    if (!newBodyArmor.isNull() && newBodyArmor.getType() != "BodyArmor")
    {
        throw InvalidEquipmentTypeException("Body armor slot requires an item of type 'BodyArmor'");
    }
    bodyArmor = newBodyArmor;
}

void Equipment::setFootArmor(const Item& newFootArmor)
{
    if (!newFootArmor.isNull() && newFootArmor.getType() != "FootArmor")
    {
        throw InvalidEquipmentTypeException("Foot armor slot requires an item of type 'FootArmor'");
    }
    footArmor = newFootArmor;
}

void Equipment::setPendant(const Item& newPendant)
{
    if (!newPendant.isNull() && newPendant.getType() != "Pendant")
    {
        throw InvalidEquipmentTypeException("Pendant slot requires an item of type 'Pendant'");
    }
    pendant = newPendant;
}

Item Equipment::findAndFillSlot(const std::string& slotType, Item item)
{
    if (slotType == "Weapon")
    {
        setWeapon(item);
        return item;
    }
    else if (slotType == "HeadArmor")
    {
        setHeadArmor(item);
        return item;
    }
    else if (slotType == "BodyArmor")
    {
        setBodyArmor(item);
        return item;
    }
    else if (slotType == "FootArmor")
    {
        setFootArmor(item);
        return item;
    }
    else if (slotType == "Pendant")
    {
        setPendant(item);
        return item;
    }
    else
    {
        throw InvalidEquipmentTypeException("Invalid equipment slot type");
    }

    return item;
}

void Equipment::equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    // Get the item from backpack (take one from the stack)
    Item itemToEquip = backpack.getItemAtTile(x, y);
    if (itemToEquip.getType() == slotType && backpack.getQuantityAtTile(x, y) == 1)
    {
        return;
    }

    // Check if the item type matches the slot type
    if (itemToEquip.getType() != slotType)
    {
        throw InvalidEquipmentTypeException("Item type doesn't match required slot type");
    }

    // Get the currently equipped item (if any)
    Item currentlyEquipped = findAndFillSlot(slotType, itemToEquip);

    // If there was a previous item
    if (!currentlyEquipped.isNull())
    {
        // Different item type at target position
        // First, try to find a non-full stack of the same item type to add to
        std::vector<std::pair<int, int>> itemTiles = backpack.findItemTile(currentlyEquipped);
        bool                             addedToExistingStack = false;

        // Loop through all tiles containing this item type with available space
        for (const auto& [itemX, itemY] : itemTiles)
        {
            // Since findItemTile returns tiles with this item type that have space available,
            // we can directly add to this tile without additional checks
            backpack.takeItemAtTile(x, y, 1);
            backpack.addItemAtTile(itemX, itemY, currentlyEquipped, 1);
            addedToExistingStack = true;

            // Successfully added to existing stack, exit the loop
            break;
        }

        // If we couldn't add to an existing stack, use an empty tile
        if (!addedToExistingStack)
        {
            // Different item type - find an empty tile to place the unequipped item
            std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();

            if (emptyTiles.empty())
            {
                // No empty tile available
                throw BackpackOvercapacityException();
            }

            // Use the first empty tile
            int emptyX = emptyTiles[0].first;
            int emptyY = emptyTiles[0].second;

            // Add the unequipped item to the empty slot
            backpack.takeItemAtTile(x, y, 1);
            backpack.addItemAtTile(emptyX, emptyY, currentlyEquipped, 1);
        }
    }
}

void Equipment::unequipItemToBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    Item itemToUnequip;

    // Get the item from the specified slot but don't remove it yet
    if (slotType == "Weapon")
    {
        itemToUnequip = weapon;
        if (itemToUnequip.isNull())
            throw EmptyCellException();
    }
    else if (slotType == "HeadArmor")
    {
        itemToUnequip = headArmor;
        if (itemToUnequip.isNull())
            throw EmptyCellException();
    }
    else if (slotType == "BodyArmor")
    {
        itemToUnequip = bodyArmor;
        if (itemToUnequip.isNull())
            throw EmptyCellException();
    }
    else if (slotType == "FootArmor")
    {
        itemToUnequip = footArmor;
        if (itemToUnequip.isNull())
            throw EmptyCellException();
    }
    else if (slotType == "Pendant")
    {
        itemToUnequip = pendant;
        if (itemToUnequip.isNull())
            throw EmptyCellException();
    }
    else
    {
        // Unknown slot type
        throw InvalidEquipmentTypeException("Invalid equipment slot type");
    }

    // First, let's try to get the item at the target slot (if any)
    // If the target tile has no item, getItemAtTile will throw EmptyCellException
    // which will propagate up from this function - that's what we want
    bool targetIsEmpty = false;

    // Rather than using try/catch, we'll scan all empty tiles to see if our target
    // coordinates match any of them
    std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();
    for (const auto& [emptyX, emptyY] : emptyTiles)
    {
        if (emptyX == x && emptyY == y)
        {
            targetIsEmpty = true;
            break;
        }
    }

    if (targetIsEmpty)
    {
        // Target is empty - simply place the unequipped item there
        backpack.addItemAtTile(x, y, itemToUnequip, 1);

        // Clear the equipment slot
        findAndFillSlot(slotType, Item());
    }
    else
    {
        Item targetItem = backpack.getItemAtTile(x, y);

        // Check if items are of the same type
        if (targetItem.getType() == itemToUnequip.getType())
        {
            // Same item type - swap only 1 from target
            // Take one item from the backpack
            Item takenItem = backpack.takeItemAtTile(x, y, 1);

            // Put the unequipped item in the backpack
            backpack.addItemAtTile(x, y, itemToUnequip, 1);

            // Fill the slot with the item we took
            findAndFillSlot(slotType, takenItem);
        }
        else
        {
            // Different item type at target position
            // First, try to find a non-full stack of the same item type to add to
            std::vector<std::pair<int, int>> itemTiles = backpack.findItemTile(itemToUnequip);
            bool                             addedToExistingStack = false;

            // Loop through all tiles containing this item type with available space
            for (const auto& [itemX, itemY] : itemTiles)
            {
                // Skip the target tile we already checked (it has a different item)
                if (itemX == x && itemY == y)
                {
                    continue;
                }

                // Since findItemTile returns tiles with this item type that have space available,
                // we can directly add to this tile without additional checks
                backpack.addItemAtTile(itemX, itemY, itemToUnequip, 1);
                addedToExistingStack = true;

                // Clear the equipment slot
                findAndFillSlot(slotType, Item());

                // Successfully added to existing stack, exit the loop
                break;
            }

            // If we couldn't add to an existing stack, use an empty tile
            if (!addedToExistingStack)
            {
                // Different item type - find an empty tile to place the unequipped item
                std::vector<std::pair<int, int>> emptyTiles = backpack.findEmptyTile();

                if (emptyTiles.empty())
                {
                    // No empty tile available
                    throw BackpackOvercapacityException();
                }

                // Use the first empty tile
                int emptyX = emptyTiles[0].first;
                int emptyY = emptyTiles[0].second;

                // Add the unequipped item to the empty slot
                backpack.addItemAtTile(emptyX, emptyY, itemToUnequip, 1);

                // Clear the equipment slot
                findAndFillSlot(slotType, Item());
            }
        }
    }
}