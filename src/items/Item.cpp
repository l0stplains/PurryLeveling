#include "items/Item.hpp"

Item::Item() : itemID(""), name(""), type(""), rarity(' '), effects({}) {}

Item::Item(std::string                                itemID,
           const std::string&                         name,
           const std::string&                         type,
           char                                       rarity,
           const std::vector<std::unique_ptr<Effect>> effects)
    : itemID(itemID), name(name), type(type), rarity(rarity), effects(effects)
{}

Item::Item(const Item& other)
    : itemID(other.itemID),
      name(other.name),
      type(other.type),
      rarity(other.rarity),
      effects(other.effects)
{}

Item& Item::operator=(const Item& other)
{
    itemID  = other.itemID;
    name    = other.name;
    type    = other.type;
    rarity  = other.rarity;
    effects = other.effects;
    return *this;
}

bool Item::operator==(const Item& other) const
{
    return itemID == other.itemID && name == other.name && type == other.type &&
           rarity == other.rarity && effects == other.effects;
}

bool Item::operator!=(const Item& other) const
{
    return itemID != other.itemID || name != other.name || type != other.type ||
           rarity != other.rarity || effects != other.effects;
}

Item::~Item() {}

std::string Item::getItemID() const
{
    return itemID;
}

std::string Item::getName() const
{
    return name;
}

std::string Item::getType() const
{
    return type;
}

char Item::getRarity() const
{
    return rarity;
}

std::vector<std::unique_ptr<Effect>> Item::getEffects() const
{
    return effects;
}

bool Item::isNull() const
{
    return itemID.empty();
}