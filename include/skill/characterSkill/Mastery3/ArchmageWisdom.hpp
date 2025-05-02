#pragma once
#include "effects/useEffects/Chronoflux.hpp"
#include "effects/useEffects/FortuneCall.hpp"
#include "skill/characterSkill/MageSkill.hpp"

// SKill Mastery3 Mage
class ArchmageWisdom : public MageSkill
{
public:
    ArchmageWisdom(bool isLearned = false, bool isActive = false)
        : MageSkill("Archmage's Wisdom", 14, 6, 12, 1.0f, {}, {}, isLearned, isActive, 0.60f, 0)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FortunesCall>(20, 0.15f, 6));
        effectVec.push_back(make_unique<Chronoflux>(0, 0.2, 3));
        this->setEffects(std::move(effectVec));
    }
    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int   getManaRegenBonus() const override { return manaRegenBonus; }
};