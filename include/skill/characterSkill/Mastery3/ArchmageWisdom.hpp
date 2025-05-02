#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "effects/useEffects/FortuneCall.hpp"
#include "skill/Skill.hpp"

// SKill Mastery3 Mage
class ArchmageWisdom : public Skill
{
private:
    float intelligenceMultiplier = 0.60f;  // Massive intelligence boost
    int   manaRegenBonus         = 0;

public:
    ArchmageWisdom(bool isLearned = false, bool isActive = false)
        : Skill("Archmage's Wisdom", 14, 6, 12, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FortunesCall>(20, 0.15f, 6));
        effectVec.push_back(make_unique<Chronoflux>(0, 0.2, 3));  // Cooldown reduction 20%
        this->setEffects(std::move(effectVec));
    }

    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int   getManaRegenBonus() const { return manaRegenBonus; }
};