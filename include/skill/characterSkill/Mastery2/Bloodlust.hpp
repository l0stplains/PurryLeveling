#pragma once
#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/Skill.hpp"
#include <memory>

// Berserker Mastery 2
class Bloodlust : public Skill
{
private:
    // Same set of attributes as parent but with updated values
    float rageMultiplier   = 0.18f;  // Enhanced rage multiplier
    float lifeStealFactor  = 0.15f;  // Adds life steal
    float selfDamage       = 0.0f;   // No self damage

public:
    Bloodlust(bool isLearned = false, bool isActivated = false) 
        : Skill("Bloodlust", 7, 2, 6, 1.0f, {}, {}, isLearned, isActivated)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Bloodthirst>(5, 0.10f, 0.05f, 3));  // +5 Str, +10% crit, 5% skip chance
        effectVec.push_back(make_unique<HarmoniousEmpowerment>(15, 5, rageMultiplier, 4));
        this->setEffects(std::move(effectVec));
    }
    
    float getRageMultiplier() const { return rageMultiplier; }
    float getLifeStealFactor() const { return lifeStealFactor; }
    float getSelfDamage() const { return selfDamage; }
};