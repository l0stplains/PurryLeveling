#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// SKill Mastery3 Mage
class EtherealBond : public Skill
{
private:
    float intelligenceMultiplier = 0.45f;  // Massive intelligence boost
    int   manaRegenBonus         = 50;

public:
    EtherealBond() : Skill("Ethereal Bond", 15, 6, 0, 1.0f, {}, {}, false, false) 
    {
        vector<Effect> effectVec;
        effectVec.push_back(Chronoflux(-10, 0.1, 3)); // Cooldown reduction 10%, mana cost reduction 10
        this->setEffects(effectVec);
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};