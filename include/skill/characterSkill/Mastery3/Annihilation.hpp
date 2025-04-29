#pragma once
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/Overclock.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/Skill.hpp"

// Berserkser Master3 Skill
class Annihilation : public Skill
{
private:
    // Same attributes as the entire tree but with ultimate values
    float rageMultiplier   = 0.40f;  // Massive rage boost
    float lifeStealFactor  = 0.0f;   // No life steal (focused on pure damage)
    float selfDamage       = 0.10f;  // Costs 10% of own health to use

public:
    Annihilation(bool isLearned = false, bool isActive = false) 
        : Skill("Annihilation", 15, 6, 25, 1.0f, {}, {}, isLearned, isActive)
    {
        // Combine two powerful effects
        vector<Effect> effectVec;
        effectVec.push_back(GladiatorMight(10, 5, 1.2f, 3));  // +10 Str, +5 PhysDef, 120% resource
        effectVec.push_back(Overclock(1.5f, 1.3f, 10, 2)); 
        effectVec.push_back(HarmoniousEmpowerment(10, 5, rageMultiplier, 3));
        this->setEffects(effectVec);
    }

    // Getters
    float getRageMultiplier() const { return rageMultiplier; }
    float getLifeStealFactor() const { return lifeStealFactor; }
    float getSelfDamage() const { return selfDamage; }
};
