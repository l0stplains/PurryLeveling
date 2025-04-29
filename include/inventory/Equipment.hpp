#pragma once

#include <string>
#include "../include/items/Item.hpp"

// Forward declarations
class Backpack;
class ItemManager;

class Equipment {
public:
    Equipment();
    Equipment(ItemManager& itemManager, const std::string& weapon, const std::string& headArmor, const std::string& bodyArmor, const std::string& footArmor, const std::string& pendant);

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
    
    bool equipItemFromBackpack(Backpack& backpack, int x, int y, const std::string& slotType);
    bool unequipItemToBackpack(Backpack& backpack, int x, int y, const std::string& slotType);

private:
    Item weapon;
    Item headArmor;
    Item bodyArmor;
    Item footArmor;
    Item pendant;
};