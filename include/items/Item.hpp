#pragma once

#include <memory>
#include <vector>

#include "effects/Effect.hpp"

class Item
{
public:
    Item();
    Item(std::string                                itemID,
         const std::string&                         name,
         const std::string&                         type,
         char                                       rarity,
         const std::vector<std::shared_ptr<Effect>> effects,
         const std::string&                         description);
    Item(const Item& other);
    Item& operator=(const Item& other);
    bool  operator==(const Item& other) const;
    bool  operator!=(const Item& other) const;
    ~Item();

    std::string                          getItemID() const;
    std::string                          getName() const;
    std::string                          getType() const;
    char                                 getRarity() const;
    std::vector<std::shared_ptr<Effect>> getEffects() const;
    bool                                 isNull() const;
    std::string                          getDescription() const;

protected:
    std::string                          itemID;
    std::string                          name;
    std::string                          type;
    char                                 rarity;
    std::vector<std::shared_ptr<Effect>> effects;
    std::string                          description;
};