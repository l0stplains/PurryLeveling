#pragma once
#include "skill/Skill.hpp"

// Skill Goblin
class SneakyStab : public Skill
{
    private:
        float damageMultiplier = 2.0f; // Damage multiplier for the skill

    public:
        SneakyStab() : Skill("Sneaky Stab", 0, 0, 0, 0.0f, {}, {}, false, false){};

        void setDamageMultiplier(float multiplier) { damageMultiplier = multiplier; }
        float getDamageMultiplier() const { return damageMultiplier; }

};