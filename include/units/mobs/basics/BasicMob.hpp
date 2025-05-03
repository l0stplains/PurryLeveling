#pragma once

#include "skill/Skill.hpp"
#include "units/mobs/Mob.hpp"

class BasicMob : virtual public Mob
{
public:
    BasicMob(const std::string& name);

    ~BasicMob() override = default;

    // Prevent copying, allow moving
    BasicMob(const BasicMob&)            = delete;
    BasicMob& operator=(const BasicMob&) = delete;
    BasicMob(BasicMob&&)                 = default;
    BasicMob& operator=(BasicMob&&)      = default;
};