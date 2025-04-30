#pragma once

#include "skill/Skill.hpp"
#include "units/Unit.hpp"

/**
 * @brief Represents an AnimatedUnit controlled by a player or with RPG stats.
 */
class Mob : virtual public Unit
{
public:
    Mob(const std::string& name, Skill& healSkill);

    ~Mob() override = default;

    // Prevent copying, allow moving
    Mob(const Mob&)            = delete;
    Mob& operator=(const Mob&) = delete;
    Mob(Mob&&)                 = default;
    Mob& operator=(Mob&&)      = default;

    // --- RPG Stats Getters ---
    int  GetLevel() const;
    void SetLevel(int);

    const Skill& GetHealSkill() const;

protected:
    // --- RPG Stats ---
    int    m_level = 1;
    Skill& m_healSkill;
};