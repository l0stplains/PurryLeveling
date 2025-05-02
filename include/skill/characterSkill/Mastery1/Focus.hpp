#pragma once
#include "skill/characterSkill/Mastery2/ArcaneInsight.hpp"
#include "skill/characterSkill/Mastery2/MentalClarity.hpp"
#include "skill/characterSkill/Mastery2/SpiritualConnection.hpp"

// Root Skill Untuk Mage
class Focus : public MageSkill
{
public:
    Focus(bool isLearned = true, bool isActivated = true)
        : MageSkill("Focus", 5, 3, 0, 0, {}, {}, isLearned, isActivated, 0.15f, 0)
    {
        vector<unique_ptr<Skill>> secondSkill;

        secondSkill.push_back(std::make_unique<ArcaneInsight>());
        secondSkill.push_back(std::make_unique<SpiritualConnection>());
        secondSkill.push_back(std::make_unique<MentalClarity>());

        this->setChildren(std::move(secondSkill));
    }

    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};