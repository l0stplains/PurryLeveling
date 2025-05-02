#pragma once

#include "skill/Skill.hpp"
#include "units/mobs/Mob.hpp"

/**
 * @brief Represents an AnimatedUnit controlled by a player or with RPG stats.
 */
class BasicMob : virtual public Mob
{
public:
    BasicMob(const std::string& name, Skill& healSkill);

    ~BasicMob() override = default;

    // Prevent copying, allow moving
    BasicMob(const BasicMob&)            = delete;
    BasicMob& operator=(const BasicMob&) = delete;
    BasicMob(BasicMob&&)                 = default;
    BasicMob& operator=(BasicMob&&)      = default;
};