#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Chronoflux.hpp"
#include <memory>

// SKill Mastery3 Mage
class EtherealBond : public Skill
{
private:
    float intelligenceMultiplier = 0.45f;  // Massive intelligence boost
    int   manaRegenBonus         = 50;

public:
    EtherealBond(bool isLearned = false, bool isActive = false) 
        : Skill("Ethereal Bond", 15, 6, 0, 1.0f, {}, {}, isLearned, isActive) 
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Chronoflux>(-10, 0.1, 3)); // Cooldown reduction 10%, mana cost reduction 10
        this->setEffects(std::move(effectVec));
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};