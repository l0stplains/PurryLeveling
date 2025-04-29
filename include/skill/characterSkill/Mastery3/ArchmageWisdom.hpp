#pragma once
#include "effects/useEffects/FortuneCall.hpp"
#include "skill/Skill.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// SKill Mastery3 Mage
class ArchmageWisdom : public Skill
{
private:
    float intelligenceMultiplier = 0.60f;  // Massive intelligence boost
    int   manaRegenBonus         = 0;

public:
    ArchmageWisdom() : Skill("Archmage's Wisdom", 14, 6, 12, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        effectVec.push_back(FortunesCall(20, 0.15f, 6));
        effectVec.push_back(Chronoflux(0, 0.2, 3)); // Cooldown reduction 20%
        this->setEffects(effectVec);
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};