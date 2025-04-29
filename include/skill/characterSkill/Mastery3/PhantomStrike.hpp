#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "effects/useEffects/Bloodthirst.hpp"

// Skill Mastery3 Assassin
class PhantomStrike : public Skill
{
private:
    // Same attributes as parent skill with enhanced values
    float agilityMultiplier = 0.25f;  // Good agility boost

public:
    PhantomStrike(bool isLearned = false, bool isActive = false) 
        : Skill("Phantom Strike", 14, 6, 25, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<Effect> effectVec;
        effectVec.push_back(TimeWarp(0.40f, 0.131f, 3));  // 20% enemy skip turn, 50% damage boost
        effectVec.push_back(Bloodthirst(40, 0.35, 0.15, 3));
        this->setEffects(effectVec);
    }
    
    // Getter
    float getAgilityMultiplier() const { return agilityMultiplier; }
};
