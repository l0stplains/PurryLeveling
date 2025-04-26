#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/FortuneCall.hpp"
#include "effects/useEffects/TimeWarp.hpp"
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
        };
};

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

class CosmicMastery : public Skill
{
    private:
        float intelligenceMultiplier = 0.35f;   // Massive intelligence boost
        float timeWarpChance = 0.40f;           // Chance to reset spell cooldown
        float spellPowerBoost = 0.0;          // Increases spell damage
        float manaCostReduction = 0.0f;        // Reduces mana costs by 0%
        int manaRegenBonus = 0;

    public:
        CosmicMastery() : Skill("Cosmic Mastery", 25, 8, 15, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(TimeWarp(0.40f, 0.25f, 5)); 
            this->setEffects(effectVec);
            // No children (leaf node)
        };
};

class EtherealBond : public Skill
{
    private:
        float intelligenceMultiplier = 0.45f;   // Massive intelligence boost
        float timeWarpChance = 0.10f;           // Chance to reset spell cooldown
        float spellPowerBoost = 0.0;          // Increases spell damage
        float manaCostReduction = 0.0f;        // Reduces mana costs by 0%
        int manaRegenBonus = 50;

    public:
        EtherealBond() : Skill("Ethereal Bond", 15, 6, 0, 1.0f, {}, {}, false, false){};
};