#include "../include/inventory/Equipment.hpp"

#include "../include/exception/Exception.hpp"
#include "../include/inventory/Backpack.hpp"
#include "../include/items/ItemManager.hpp"

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
    try
    {
        if (!weaponID.empty())
        {
            Item weaponItem = itemManager.getItem(weaponID);
            if (weaponItem.getType() == "Weapon")
            {
                this->weapon = weaponItem;
            }
        }
    }
    catch (const char* e)
    {
        // Item not found, leave as null
    }

    try
    {
        if (!headArmorID.empty())
        {
            Item headItem = itemManager.getItem(headArmorID);
            if (headItem.getType() == "HeadArmor")
            {
                this->headArmor = headItem;
            }
        }
    }
    catch (const char* e)
    {
        // Item not found, leave as null
    }

    try
    {
        if (!bodyArmorID.empty())
        {
            Item bodyItem = itemManager.getItem(bodyArmorID);
            if (bodyItem.getType() == "BodyArmor")
            {
                this->bodyArmor = bodyItem;
            }
        }
    }
    catch (const char* e)
    {
        // Item not found, leave as null
    }

    try
    {
        if (!footArmorID.empty())
        {
            Item footItem = itemManager.getItem(footArmorID);
            if (footItem.getType() == "FootArmor")
            {
                this->footArmor = footItem;
            }
        }
    }
    catch (const char* e)
    {
        // Item not found, leave as null
    }

    try
    {
        if (!pendantID.empty())
        {
            Item pendantItem = itemManager.getItem(pendantID);
            if (pendantItem.getType() == "Pendant")
            {
                this->pendant = pendantItem;
            }
        }
    }
    catch (const char* e)
    {
        // Item not found, leave as null
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

// Integration with backpack - updated to use new backpack methods
bool Equipment::equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    try
    {
        // Get the item from backpack (take one from the stack)
        Item itemToEquip = backpack.takeItemAtTile(x, y, 1);

        // Check if the item type matches the slot type
        if (itemToEquip.getType() != slotType)
        {
            // Return the item to backpack if types don't match
            backpack.addItemAtTile(x, y, itemToEquip, 1);
            return false;
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
            return false;
        }

        // If there was a previous item, return it to backpack
        if (!currentlyEquipped.isNull())
        {
            backpack.addItemAtTile(x, y, currentlyEquipped, 1);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        // If any error occurs, the operation fails
        return false;
    }
}

bool Equipment::unequipItemToBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    try
    {
        Item itemToUnequip;

        // Get the item from the specified slot but don't remove it yet
        if (slotType == "Weapon")
        {
            itemToUnequip = weapon;
            if (itemToUnequip.isNull())
                return false;
        }
        else if (slotType == "HeadArmor")
        {
            itemToUnequip = headArmor;
            if (itemToUnequip.isNull())
                return false;
        }
        else if (slotType == "BodyArmor")
        {
            itemToUnequip = bodyArmor;
            if (itemToUnequip.isNull())
                return false;
        }
        else if (slotType == "FootArmor")
        {
            itemToUnequip = footArmor;
            if (itemToUnequip.isNull())
                return false;
        }
        else if (slotType == "Pendant")
        {
            itemToUnequip = pendant;
            if (itemToUnequip.isNull())
                return false;
        }
        else
        {
            // Unknown slot type
            return false;
        }

        // Check if the target backpack slot is empty
        bool targetIsEmpty = false;
        Item targetItem;
        int  targetQuantity = 0;

        try
        {
            targetItem     = backpack.getItemAtTile(x, y);
            targetQuantity = backpack.getQuantityAtTile(x, y);
            // If we reach here, the target is not empty
            targetIsEmpty = false;
        }
        catch (const EmptyCellException&)
        {
            // Target is empty, proceed with unequipping
            targetIsEmpty = true;
        }
        catch (...)
        {
            // Other errors
            return false;
        }

        if (targetIsEmpty)
        {
            // Add the item to the backpack first
            backpack.addItemAtTile(x, y, itemToUnequip, 1);

            // Only after successful backpack operation, clear the equipment slot
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

            return true;
        }
        else
        {
            // Target is not empty, only can swap if quantity target is 1
            // and target has the same type as the equipped item
            if (targetQuantity == 1 && targetItem.getType() == itemToUnequip.getType())
            {
                try
                {
                    // Take the item from the backpack first
                    Item takenItem = backpack.takeItemAtTile(x, y, 1);

                    // Then try to put the unequipped item in the backpack
                    backpack.addItemAtTile(x, y, itemToUnequip, 1);

                    // Only now update the equipment with the taken item
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

                    return true;
                }
                catch (const std::exception& e)
                {
                    // If any of the backpack operations fail, don't modify equipment
                    return false;
                }
            }
            return false;
        }
    }
    catch (const std::exception& e)
    {
        // If any error occurs, the operation fails
        return false;
    }
}