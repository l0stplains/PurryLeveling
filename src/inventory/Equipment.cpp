#include "inventory/Equipment.hpp"
#include <iostream>

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

        if (itemID == "NOL")
        {
            continue;
        }
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

Item& Equipment::findSlot(const std::string& slotType)
{
    if (slotType == "Weapon")
    {
        return weapon;
    }
    else if (slotType == "HeadArmor")
    {
        return headArmor;
    }
    else if (slotType == "BodyArmor")
    {
        return bodyArmor;
    }
    else if (slotType == "FootArmor")
    {
        return footArmor;
    }
    else if (slotType == "Pendant")
    {
        return pendant;
    }
    else
    {
        throw InvalidEquipmentTypeException("Invalid equipment slot type");
    }
}

void Equipment::equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{   
    Item itemToEquip = backpack.getItemAtTile(x, y);
    Item& slot = findSlot(slotType);

    if (itemToEquip.getType() != slotType)
    {
        throw InvalidEquipmentTypeException("Item type doesn't match required slot type");
    }
    else 
    {
        backpack.takeItem(itemToEquip, 1);
    }

    if (!slot.isNull())
    {
        unequipItemToBackpack(backpack, x, y, slotType);
    }
    slot = itemToEquip;
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

    Item targetItem;
    try
    {
        targetItem = backpack.getItemAtTile(x, y);
    }
    catch (const EmptyCellException&)
    {
        targetIsEmpty = true;
    }

    std::cout << "Target is empty: " << targetIsEmpty << std::endl;

    if (targetIsEmpty)
    {
        // Target is empty - simply place the unequipped item there
        backpack.addItemAtTile(x, y, itemToUnequip, 1);

        // Clear the equipment slot
        findSlot(slotType) = Item();
    }
    else
    {
        // Check if items are of the same type
        if (targetItem.getType() == itemToUnequip.getType())
        {
            // Same item type - swap only 1 from target
            if (backpack.getQuantityAtTile(x, y) == 1)
            {
                backpack.takeItemAtTile(x, y, 1);
                backpack.addItemAtTile(x, y, itemToUnequip, 1);
            }
            else
            {
                backpack.takeItemAtTile(x, y, 1);
                backpack.addItem(itemToUnequip, 1);
            }
            findSlot(slotType) = targetItem;
        }
        else
        {
            backpack.addItem(itemToUnequip, 1);
            findSlot(slotType) = Item();
        }
    }
}