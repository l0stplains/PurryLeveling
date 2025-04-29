#pragma once
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/Skill.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// SKill Mastery3 Mage
class CosmicMastery : public Skill
{
private:
    float intelligenceMultiplier = 0.35f;  // Massive intelligence boost
    int   manaRegenBonus         = 20;

public:
    CosmicMastery(bool isLearned = false, bool isActive = false) 
        : Skill("Cosmic Mastery", 25, 8, 15, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<Effect> effectVec;
        effectVec.push_back(TimeWarp(0.40f, 0.25f, 5));
        effectVec.push_back(Chronoflux(-10, 0.4, 3)); // 40% pengurangan cooldown
        this->setEffects(effectVec);
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};