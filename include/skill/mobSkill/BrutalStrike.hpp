#pragma once
#include "skill/Skill.hpp"

// Skill untuk Orc
class BrutalStrike : public Skill
{
private:
    float damageMultiplier = 1.5f;
    float stunChance       = 0.5f; // 50%

public:
    BrutalStrike() : Skill("Brutal Strike", 0, 0, 0, 0.0f, {}, {}, false, false) {};

    float getDamageMultiplier() const { return damageMultiplier; }
    float getStunChance() const { return stunChance; }
};
