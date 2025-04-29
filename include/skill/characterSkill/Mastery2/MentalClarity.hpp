#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/ManaMastery.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// Mage Master 2
class MentalClarity : public Skill
{
private:
    float intelligenceMultiplier = 0.15f;  // Enhanced intelligence
    int   manaRegenBonus         = 0;

public:
    MentalClarity() : Skill("Mental Clarity", 7, 2, 0, 1.0f, {}, {}, false, false)
    {
        vector<Skill*> thirdSkill;

        ManaMastery* manaMastery = new ManaMastery();
        thirdSkill.push_back(manaMastery);
        this->setChildren(thirdSkill);

        vector<Effect> effectVec;
        effectVec.push_back(Chronoflux(0.2, 0, 3));
        this->setEffects(effectVec);
    }
    
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};
