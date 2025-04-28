#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/FortuneCall.hpp"

class ArchmageWisdom : public Skill
{
    private:
        float intelligenceMultiplier = 0.60f;   // Massive intelligence boost
        float timeWarpChance = 0.20f;           // Chance to reset spell cooldown
        float spellPowerBoost = 0.4f;          // Increases spell damage
        float manaCostReduction = 0.0f;        // Reduces mana costs by 0%
        int manaRegenBonus = 0;

    public:
        ArchmageWisdom() : Skill("Archmage's Wisdom", 14, 6, 12, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(FortunesCall(20, 0.15f, 6));
            this->setEffects(effectVec);
            // No children (leaf node)
        };
};