#pragma once
#include "skill/Skill.hpp"

class Guardian : public Skill
{
    private:
        float strengthMultiplier = 0.20f;     // Moderate strength boost
        float damageReduction = 0.35f;        // Major damage reduction
        float reflectDamage = 0.20f;          // Reflects portion of damage
        float critChanceBonus = 0.25f;       // Increased critical hit chance
        float damageBoost = 0.0f;

    public:
        Guardian() : Skill("Guardian", 15, 6, 5, 1.0f, {}, {}, false, false)
        {
        };
};