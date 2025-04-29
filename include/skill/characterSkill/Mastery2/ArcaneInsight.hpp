#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/ArchmageWisdom.hpp"
#include "effects/useEffects/Chronoflux.hpp"

// Mage Skill Mastery 2
class ArcaneInsight : public Skill
{
private:
    float intelligenceMultiplier = 0.20f; 
    int   manaRegenBonus         = 0;

public:
    ArcaneInsight() : Skill("Arcane Insight", 9, 3, 6, 1.0f, {}, {}, false, false)
    {
        vector<Skill*> thirdSkill;

        ArchmageWisdom* archmageWisdom = new ArchmageWisdom();
        thirdSkill.push_back(archmageWisdom);
        this->setChildren(thirdSkill);

        vector<Effect> effectVec;
        effectVec.push_back(Chronoflux(0, 0, 3));
        this->setEffects(effectVec);
    }
    
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};
