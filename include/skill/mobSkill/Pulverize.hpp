#pragma once
#include "skill/Skill.hpp"

// Skill Boss Ogre
class Pulverize : public Skill
{
private:
    float damageMultiplier = 2.5f;  // Damage multiplier for the skill
    float stunChance       = 0.9f;  // Chance to stun

public:
    Pulverize(bool isLearned = false, bool isActivated = false)
        : Skill("Pulverize", 0, 0, 0, 0.0f, {}, {}, isLearned, isActivated) {};

    float getDamageMultiplier() const { return damageMultiplier; }
    float getStunChance() const { return stunChance; }
};