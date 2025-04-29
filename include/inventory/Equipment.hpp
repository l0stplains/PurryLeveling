#pragma once

#include "inventory/Backpack.hpp"
#include "items/ItemManager.hpp"

class Equipment
{
public:
    /**
     * @brief Default constructor for Equipment
     * @return Equipment object
     */
    Equipment();

    /**
     * @brief Constructor for Equipment with specific items
     * @param itemManager ItemManager object
     * @param weaponID ID of the weapon item
     * @param headArmorID ID of the head armor item
     * @param bodyArmorID ID of the body armor item
     * @param footArmorID ID of the foot armor item
     * @param pendantID ID of the pendant item
     * @return Equipment object
     */
    Equipment(ItemManager&       itemManager,
              const std::string& weaponID,
              const std::string& headArmorID,
              const std::string& bodyArmorID,
              const std::string& footArmorID,
              const std::string& pendantID);

    Item getWeapon() const;
    Item getHeadArmor() const;
    Item getBodyArmor() const;
    Item getFootArmor() const;
    Item getPendant() const;

    void setWeapon(const Item& weapon);
    void setHeadArmor(const Item& headArmor);
    void setBodyArmor(const Item& bodyArmor);
    void setFootArmor(const Item& footArmor);
    void setPendant(const Item& pendant);

    /**
     * @brief Equip an item from the backpack to the equipment
     * @param backpack Backpack object
     * @param x x coordinate in the backpack
     * @param y y coordinate in the backpack
     * @param slotType Type of the equipment slot (e.g., "Weapon", "HeadArmor", etc.)
     */
    void equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType);

    /**
     * @brief Unequip an item from the equipment to the backpack
     * @param backpack Backpack object
     * @param x x coordinate in the backpack
     * @param y y coordinate in the backpack
     * @param slotType Type of the equipment slot (e.g., "Weapon", "HeadArmor", etc.)
     */
    void unequipItemToBackpack(Backpack& backpack, int x, int y, const std::string& slotType);

private:
    Item weapon;
    Item headArmor;
    Item bodyArmor;
    Item footArmor;
    Item pendant;
};