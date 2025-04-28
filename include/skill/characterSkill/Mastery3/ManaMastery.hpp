#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/MysticBarrier.hpp"

class ManaMastery : public Skill
{
    private:
        float intelligenceMultiplier = 0.35f;   // Massive intelligence boost
        float timeWarpChance = 0.05f;           // Chance to reset spell cooldown
        float spellPowerBoost = 0.20;          // Increases spell damage
        float manaCostReduction = 0.4f;        // Reduces mana costs by 0%
        int manaRegenBonus = 5;

    public:
        ManaMastery() : Skill("Mana Mastery", 12, 5, 0, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(MysticBarrier(30, 16.0, 0.10f, 7));  // 30% magic damage reduction for 4 turns
            this->setEffects(effectVec);
            // No children (leaf node)
        }
};