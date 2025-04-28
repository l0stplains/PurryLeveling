#pragma once

#include <string>
#include <vector>

class Item {
    public:
        Item();
        Item(std::string itemID, const std::string& name, const std::string& type, char rarity, const std::vector<std::string>& effects);
        Item(const Item& other);
        Item& operator=(const Item& other);
        bool operator==(const Item& other) const;
        bool operator!=(const Item& other) const;
        ~Item();

        std::string getItemID() const;
        std::string getName() const;
        std::string getType() const;
        char getRarity() const;
        std::vector<std::string> getEffects() const;
        bool isNull() const;

    protected:
        std::string itemID;
        std::string name;
        std::string type;
        char rarity;
        std::vector<std::string> effects;
};