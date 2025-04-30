#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/Skill.hpp"

// SKill Mastery3 Mage
class CosmicMastery : public Skill
{
private:
    float intelligenceMultiplier = 0.35f;  // Massive intelligence boost
    int   manaRegenBonus         = 20;

public:
    CosmicMastery(bool isLearned = false, bool isActive = false)
        : Skill("Cosmic Mastery", 25, 8, 15, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<TimeWarp>(0.40f, 0.25f, 5));
        effectVec.push_back(make_unique<Chronoflux>(-10, 0.4, 3));  // 40% pengurangan cooldown
        this->setEffects(std::move(effectVec));
    }

    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int   getManaRegenBonus() const { return manaRegenBonus; }
};