#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "skill/characterSkill/MageSkill.hpp"

// SKill Mastery3 Mage
class EtherealBond : public MageSkill
{
public:
    EtherealBond(bool isLearned = false, bool isActive = false)
        : MageSkill("Ethereal Bond", 15, 6, 12, 0.7f, {}, {}, isLearned, isActive, 0.45f, 50)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Chronoflux>(-10, 0.1, 3));
        this->setEffects(std::move(effectVec));
    }

    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};