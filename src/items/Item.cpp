#include "items/Item.hpp"

Item::Item() : itemID("NOL"), name(""), type(""), rarity(' '), effects({}), description("") {}

Item::Item(std::string                                itemID,
           const std::string&                         name,
           const std::string&                         type,
           char                                       rarity,
           const std::vector<std::shared_ptr<Effect>> effects,
           const std::string&                         description)
    : itemID(itemID), name(name), type(type), rarity(rarity), effects(effects), description(description)
{}

Item::Item(const Item& other)
    : itemID(other.itemID),
      name(other.name),
      type(other.type),
      rarity(other.rarity),
      effects(other.effects),
      description(other.description)
{}

Item& Item::operator=(const Item& other)
{
    itemID      = other.itemID;
    name        = other.name;
    type        = other.type;
    rarity      = other.rarity;
    effects     = other.effects;
    description = other.description;
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

std::vector<std::shared_ptr<Effect>> Item::getEffects() const
{
    return effects;
}

std::string Item::getDescription() const
{
    return description;
}

bool Item::isNull() const
{
    return itemID == "NOL";
}