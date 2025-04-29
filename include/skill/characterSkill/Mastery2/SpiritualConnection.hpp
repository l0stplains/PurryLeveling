#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/EtherealBond.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// Mage Mastery 2
class SpiritualConnection : public Skill
{
private:
    float intelligenceMultiplier = 0.14f;  // Moderate intelligence boost
    int   manaRegenBonus         = 30;     // Mana regeneration

public:
    SpiritualConnection() : Skill("Spiritual Connection", 8, 3, 0, 1.0f, {}, {}, false, false)
    {
        vector<Skill*> thirdSkill;

        EtherealBond* etherealBond = new EtherealBond();
        thirdSkill.push_back(etherealBond);
        this->setChildren(thirdSkill);

        vector<Effect> effectVec;
        effectVec.push_back(Chronoflux(0, 0.3, 3));
        this->setEffects(effectVec);
    }
    
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};
