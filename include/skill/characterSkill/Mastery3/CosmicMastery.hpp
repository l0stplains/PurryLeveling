#pragma once
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/Skill.hpp"

class CosmicMastery : public Skill
{
private:
    float intelligenceMultiplier = 0.35f;  // Massive intelligence boost
    float timeWarpChance         = 0.40f;  // Chance to reset spell cooldown
    float spellPowerBoost        = 0.0;    // Increases spell damage
    float manaCostReduction      = 0.0f;   // Reduces mana costs by 0%
    int   manaRegenBonus         = 0;

public:
    CosmicMastery() : Skill("Cosmic Mastery", 25, 8, 15, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        effectVec.push_back(TimeWarp(0.40f, 0.25f, 5));
        this->setEffects(effectVec);
        // No children (leaf node)
    };
};