#pragma once
#include "effects/useEffects/Chronoflux.hpp"
#include "skill/characterSkill/MageSkill.hpp"
#include "skill/characterSkill/Mastery3/ManaMastery.hpp"

// Mage Mastery 2
class MentalClarity : public MageSkill
{
public:
    MentalClarity(bool isLearned = false, bool isActivated = false)
        : MageSkill("Mental Clarity", 7, 2, 0, 0.26f, {}, {}, isLearned, isActivated, 0.15f, 0)
    {
        vector<unique_ptr<Skill>> thirdSkill;
        thirdSkill.push_back(std::make_unique<ManaMastery>());
        this->setChildren(std::move(thirdSkill));

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<Chronoflux>(0.2, 0, 3));
        this->setEffects(std::move(effectVec));
    }

    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};
