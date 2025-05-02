#pragma once
#include "skill/Skill.hpp"

// Skill Goblin
class SneakyStab : public Skill
{
private:
    float damageMultiplier = 2.0f;  // Damage multiplier for the skill

public:
    SneakyStab(bool isLearned = false, bool isActivated = false)
        : Skill("Sneaky Stab", 0, 0, 0, 0.0f, {}, {}, isLearned, isActivated) {};

    void  setDamageMultiplier(float multiplier) { damageMultiplier = multiplier; }
    float getDamageMultiplier() const { return damageMultiplier; }
};