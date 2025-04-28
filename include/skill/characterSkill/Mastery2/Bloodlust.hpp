#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/BattleFocus.hpp"

class Bloodlust : public Skill
{
    private:
        // Same set of attributes as parent but with updated values
        float rageMultiplier = 0.18f;          // Enhanced rage multiplier
        float lifeStealFactor = 0.15f;         // Adds life steal
        float armorPenetration = 0.0f;         // No armor penetration 
        float selfDamage = 0.0f;               // No self damage

    public:
        Bloodlust() : Skill("Bloodlust", 7, 2, 6, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(Bloodthirst(5, 0.10f, 0.05f, 3)); // +5 Str, +10% crit, 5% skip chance, 3 turns
            this->setEffects(effectVec);
        }
};