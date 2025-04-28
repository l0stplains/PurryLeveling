#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/Overclock.hpp"

class Annihilation : public Skill
{
    private:
        // Same attributes as the entire tree but with ultimate values
        float rageMultiplier = 0.40f;          // Massive rage boost
        float lifeStealFactor = 0.0f;          // No life steal (focused on pure damage)
        float armorPenetration = 0.30f;        // Increased armor penetration
        float selfDamage = 0.10f;              // Costs 10% of own health to use

    public:
        Annihilation() : Skill("Annihilation", 15, 6, 25, 1.0f, {}, {}, false, false)
        {
            // Combine two powerful effects
            vector<Effect> effectVec;
            effectVec.push_back(GladiatorMight(10, 5, 1.2f, 3)); // +10 Str, +5 PhysDef, 120% resource cost, 3 turns
            effectVec.push_back(Overclock(1.5f, 1.3f, 10, 2)); // 150% haste, 130% cost, -10 PhysDef, 2 turns
            this->setEffects(effectVec);
        }
};
