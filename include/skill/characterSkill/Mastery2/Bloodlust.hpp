#pragma once
#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/Skill.hpp"

// Berserker Mastery 2
class Bloodlust : public Skill
{
private:
    // Same set of attributes as parent but with updated values
    float rageMultiplier   = 0.18f;  // Enhanced rage multiplier
    float lifeStealFactor  = 0.15f;  // Adds life steal
    float selfDamage       = 0.0f;   // No self damage

public:
    Bloodlust() : Skill("Bloodlust", 7, 2, 6, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        effectVec.push_back(Bloodthirst(5, 0.10f, 0.05f, 3));  // +5 Str, +10% crit, 5% skip chance,
        effectVec.push_back(HarmoniousEmpowerment(15, 5, rageMultiplier, 4)); 
        this->setEffects(effectVec);
    }
    
    float getRageMultiplier() const { return rageMultiplier; }
    float getLifeStealFactor() const { return lifeStealFactor; }
    float getSelfDamage() const { return selfDamage; }
};