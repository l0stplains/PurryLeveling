#include "inventory/Equipment.hpp"

Equipment::Equipment()
{
    this->weapon    = Item();
    this->headArmor = Item();
    this->bodyArmor = Item();
    this->footArmor = Item();
    this->pendant   = Item();
}

Equipment::Equipment(ItemManager&       itemManager,
                     const std::string& weaponID,
                     const std::string& headArmorID,
                     const std::string& bodyArmorID,
                     const std::string& footArmorID,
                     const std::string& pendantID)
    : Equipment()
{
    // Call default constructor to initialize all items as null
    // Try to find and equip each item if the ID is not empty
    if (!weaponID.empty())
    {
        Item weaponItem = itemManager.getItem(weaponID);
        if (weaponItem.getType() == "Weapon")
        {
            this->weapon = weaponItem;
        }
    }

    if (!headArmorID.empty())
    {
        Item headItem = itemManager.getItem(headArmorID);
        if (headItem.getType() == "HeadArmor")
        {
            this->headArmor = headItem;
        }
    }

    if (!bodyArmorID.empty())
    {
        Item bodyItem = itemManager.getItem(bodyArmorID);
        if (bodyItem.getType() == "BodyArmor")
        {
            this->bodyArmor = bodyItem;
        }
    }

    if (!footArmorID.empty())
    {
        Item footItem = itemManager.getItem(footArmorID);
        if (footItem.getType() == "FootArmor")
        {
            this->footArmor = footItem;
        }
    }

    if (!pendantID.empty())
    {
        Item pendantItem = itemManager.getItem(pendantID);
        if (pendantItem.getType() == "Pendant")
        {
            this->pendant = pendantItem;
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

void Equipment::equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    // Get the item from backpack (take one from the stack)
    Item itemToEquip = backpack.takeItemAtTile(x, y, 1);

    // Check if the item type matches the slot type
    if (itemToEquip.getType() != slotType)
    {
        // Return the item to backpack if types don't match
        backpack.addItemAtTile(x, y, itemToEquip, 1);
        throw InvalidEquipmentTypeException("Item type doesn't match required slot type");
    }

    // Get the currently equipped item (if any)
    Item currentlyEquipped;

    // Equip the new item and store the previously equipped item
    if (slotType == "Weapon")
    {
        currentlyEquipped = weapon;
        setWeapon(itemToEquip);
    }
    else if (slotType == "HeadArmor")
    {
        currentlyEquipped = headArmor;
        setHeadArmor(itemToEquip);
    }
    else if (slotType == "BodyArmor")
    {
        currentlyEquipped = bodyArmor;
        setBodyArmor(itemToEquip);
    }
    else if (slotType == "FootArmor")
    {
        currentlyEquipped = footArmor;
        setFootArmor(itemToEquip);
    }
    else if (slotType == "Pendant")
    {
        currentlyEquipped = pendant;
        setPendant(itemToEquip);
    }
    else
    {
        // Unknown slot type
        backpack.addItemAtTile(x, y, itemToEquip, 1);
        throw InvalidEquipmentTypeException("Invalid equipment slot type");
    }

    // If there was a previous item, return it to backpack
    if (!currentlyEquipped.isNull())
    {
        backpack.addItemAtTile(x, y, currentlyEquipped, 1);
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
        if (slotType == "Weapon")
        {
            setWeapon(Item());
        }
        else if (slotType == "HeadArmor")
        {
            setHeadArmor(Item());
        }
        else if (slotType == "BodyArmor")
        {
            setBodyArmor(Item());
        }
        else if (slotType == "FootArmor")
        {
            setFootArmor(Item());
        }
        else if (slotType == "Pendant")
        {
            setPendant(Item());
        }
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

            // Update the equipment with the taken item
            if (slotType == "Weapon")
            {
                setWeapon(takenItem);
            }
            else if (slotType == "HeadArmor")
            {
                setHeadArmor(takenItem);
            }
            else if (slotType == "BodyArmor")
            {
                setBodyArmor(takenItem);
            }
            else if (slotType == "FootArmor")
            {
                setFootArmor(takenItem);
            }
            else if (slotType == "Pendant")
            {
                setPendant(takenItem);
            }
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
                if (slotType == "Weapon")
                {
                    setWeapon(Item());
                }
                else if (slotType == "HeadArmor")
                {
                    setHeadArmor(Item());
                }
                else if (slotType == "BodyArmor")
                {
                    setBodyArmor(Item());
                }
                else if (slotType == "FootArmor")
                {
                    setFootArmor(Item());
                }
                else if (slotType == "Pendant")
                {
                    setPendant(Item());
                }

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
                if (slotType == "Weapon")
                {
                    setWeapon(Item());
                }
                else if (slotType == "HeadArmor")
                {
                    setHeadArmor(Item());
                }
                else if (slotType == "BodyArmor")
                {
                    setBodyArmor(Item());
                }
                else if (slotType == "FootArmor")
                {
                    setFootArmor(Item());
                }
                else if (slotType == "Pendant")
                {
                    setPendant(Item());
                }
            }
        }
    }
}