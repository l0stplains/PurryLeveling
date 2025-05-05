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

Item Equipment::findAndFillSlot(const std::string& slotType, Item item)
{
    Item& slot    = findSlot(slotType);
    Item  oldItem = slot;
    slot          = item;
    return oldItem;
}

void Equipment::equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    Item itemToEquip;
    int  quantity;

    itemToEquip = backpack.getItemAtTile(x, y);
    quantity    = backpack.getQuantityAtTile(x, y);

    if (itemToEquip.getType() != slotType)
    {
        throw InvalidEquipmentTypeException("Item type doesn't match required slot type");
    }

    Item& slot = findSlot(slotType);

    if (!slot.isNull() && slot == itemToEquip)
        return;

    Item oldEquippedItem = slot;
    backpack.takeItemAtTile(x, y, 1);
    slot = itemToEquip;

    if (!oldEquippedItem.isNull())
    {
        try
        {
            if (quantity == 1)
            {
                backpack.addItemAtTile(x, y, oldEquippedItem, 1);
            }
            else
            {
                // This might throw BackpackOvercapacityException, which we let propagate up
                backpack.addItem(oldEquippedItem, 1);
            }
        }
        catch (const std::exception&)
        {
            // For other exceptions, try the general add method
            // Note: BackpackOvercapacityException will propagate up from here
            backpack.addItem(oldEquippedItem, 1);
        }
    }
}

void Equipment::unequipItemToBackpack(Backpack& backpack, int x, int y, const std::string& slotType)
{
    Item& equipmentSlot = findSlot(slotType);

    if (equipmentSlot.isNull())
    {
        throw EmptyCellException();
    }

    Item itemToUnequip = equipmentSlot;

    try
    {
        bool targetHasItem = false;
        Item targetItem;
        int  targetQuantity = 0;

        try
        {
            targetItem     = backpack.getItemAtTile(x, y);
            targetQuantity = backpack.getQuantityAtTile(x, y);
            targetHasItem  = true;
        }
        catch (const EmptyCellException&)
        {
            targetHasItem = false;
        }

        if (!targetHasItem)
        {
            backpack.addItemAtTile(x, y, itemToUnequip, 1);
            equipmentSlot = Item();
        }
        else if (targetItem == itemToUnequip)
        {
            try
            {
                backpack.addItemAtTile(x, y, itemToUnequip, 1);
                equipmentSlot = Item();
            }
            catch (const std::exception&)
            {
                backpack.addItem(itemToUnequip, 1);
                equipmentSlot = Item();
            }
        }
        else if (targetItem.getType() == slotType)
        {
            // Target has a different item of the same equipment type
            // Swap them: take the target item and equip it
            Item newEquipItem = backpack.takeItemAtTile(x, y, 1);
            backpack.addItemAtTile(x, y, itemToUnequip, 1);
            equipmentSlot = newEquipItem;
        }
        else
        {
            // Target has an incompatible item - find another place for the unequipped item
            backpack.addItem(itemToUnequip, 1);
            equipmentSlot = Item();
        }
    }
    catch (const std::exception& e)
    {
        // Handle failure to unequip by finding any available slot
        // Note: BackpackOvercapacityException will propagate up from here
        backpack.addItem(itemToUnequip, 1);
        equipmentSlot = Item();
    }
}