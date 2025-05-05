#pragma once

#include "skill/mobSkill/Healing.hpp"
#include "units/Unit.hpp"

class Mob : virtual public Unit
{
public:
    Mob(const std::string& name);

    ~Mob() override = default;

    // Prevent copying, allow moving
    Mob(const Mob&)            = delete;
    Mob& operator=(const Mob&) = delete;
    Mob(Mob&&)                 = default;
    Mob& operator=(Mob&&)      = default;

    // --- RPG Stats Getters ---
    int   GetLevel() const;
    float GetSkillProbabability() const;
    void  SetLevel(int);  // virtual so it can be adjusted by basic or boss

protected:
    // --- RPG Stats ---
    int   m_level              = 1;
    float m_skillProbabability = 0.f;
};