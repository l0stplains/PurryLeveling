#pragma once
#include "effects/useEffects/Chronoflux.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/characterSkill/MageSkill.hpp"

// SKill Mastery3 Mage
class CosmicMastery : public MageSkill
{
public:
    CosmicMastery(bool isLearned = false, bool isActive = false)
        : MageSkill("Cosmic Mastery", 25, 8, 15, 1.0f, {}, {}, isLearned, isActive, 0.35f, 20)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<TimeWarp>(0.40f, 0.25f, 5));
        effectVec.push_back(make_unique<Chronoflux>(-10, 0.4, 3));
        this->setEffects(std::move(effectVec));
    }
    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};