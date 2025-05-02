#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "effects/useEffects/MysticBarrier.hpp"
#include "skill/characterSkill/MageSkill.hpp"

// SKill Mastery3 Mage
class ManaMastery : public MageSkill
{
public:
    ManaMastery(bool isLearned = false, bool isActive = false)
        : MageSkill("Mana Mastery", 12, 5, 0, 1.0f, {}, {}, isLearned, isActive, 0.35f, 5)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<MysticBarrier>(30, 16.0, 0.10f, 7));
        effectVec.push_back(make_unique<Chronoflux>(-5, 0.05, 3));
        this->setEffects(std::move(effectVec));
    }
    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};