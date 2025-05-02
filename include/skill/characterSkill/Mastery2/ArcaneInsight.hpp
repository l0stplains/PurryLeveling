#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/ArchmageWisdom.hpp"

// Mage Skill Mastery 2
class ArcaneInsight : public Skill
{
private:
    float intelligenceMultiplier = 0.20f;
    int   manaRegenBonus         = 0;

public:
    ArcaneInsight(bool isLearned = false, bool isActivated = false)
        : Skill("Arcane Insight", 9, 3, 6, 1.0f, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;

        ArchmageWisdom* archmageWisdom = new ArchmageWisdom();
        thirdSkill.push_back(archmageWisdom);
        this->setChildren(thirdSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Chronoflux>(0, 0, 3));
        this->setEffects(std::move(effectVec));
    }

    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int   getManaRegenBonus() const { return manaRegenBonus; }
};
