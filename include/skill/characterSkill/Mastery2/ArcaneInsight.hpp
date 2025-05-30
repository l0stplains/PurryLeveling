#pragma once
#include "effects/useEffects/Chronoflux.hpp"
#include "skill/characterSkill/Mastery3/ArchmageWisdom.hpp"

// Mage Skill Mastery 2
class ArcaneInsight : public MageSkill
{
public:
    ArcaneInsight(bool isLearned = false, bool isActivated = false)
        : MageSkill("Arcane Insight", 9, 3, 6, 0.3f, {}, {}, isLearned, isActivated, 0.20f, 0)
    {
        vector<unique_ptr<Skill>> thirdSkill;
        thirdSkill.push_back(std::make_unique<ArchmageWisdom>());
        this->setChildren(std::move(thirdSkill));

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<Chronoflux>(0, 0, 3));
        this->setEffects(std::move(effectVec));
    }

    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};
