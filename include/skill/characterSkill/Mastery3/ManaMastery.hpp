#pragma once
#include "effects/useEffects/MysticBarrier.hpp"
#include "skill/Skill.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// SKill Mastery3 Mage
class ManaMastery : public Skill
{
private:
    float intelligenceMultiplier = 0.35f;  // Massive intelligence boost
    int   manaRegenBonus         = 5;

public:
    ManaMastery(bool isLearned = false, bool isActive = false) 
        : Skill("Mana Mastery", 12, 5, 0, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<Effect> effectVec;
        effectVec.push_back(MysticBarrier(30, 16.0, 0.10f, 7));  // 30% magic damage reduction for 4
        effectVec.push_back(Chronoflux(-5, 0.05, 3)); // Cooldown reduction 5%, mana cost reduction 5
        this->setEffects(effectVec);
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};