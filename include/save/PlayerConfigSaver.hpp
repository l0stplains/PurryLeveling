#pragma once

#include <string>

#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
#include "units/characters/Assassin.hpp"
#include "units/characters/Berseker.hpp"
#include "units/characters/Character.hpp"
#include "units/characters/Fighter.hpp"
#include "units/characters/Mage.hpp"
#include "units/characters/Necromancer.hpp"

/**
 * @class PlayerConfigSaver
 * @brief Persists equipment, backpack and skilltree under a folder named by Character::GetName().
 */
class PlayerConfigSaver
{
public:
    /**
     * @param rootDir    Base output directory (e.g. "data/configtest")
     * @param character  Character whose name determines the sub-folder
     * @param equipment  Equipment model, exposing slot→itemID map
     * @param backpack   Backpack model, exposing a grid of <Item,int>
     * @param skillTree  SkillTree model, exposing skillID→level pairs
     * @param lastError  receives an error message on failure
     */
    static bool SaveToFolder(const std::string& rootDir,
                             const Character&   character,
                             const Equipment&   equipment,
                             const Backpack&    backpack,
                             const Skill&       skillTree,
                             std::string&       lastError);
};