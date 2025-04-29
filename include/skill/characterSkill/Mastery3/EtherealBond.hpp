#pragma once
#include "skill/Skill.hpp"

class EtherealBond : public Skill
{
private:
    float intelligenceMultiplier = 0.45f;  // Massive intelligence boost
    float timeWarpChance         = 0.10f;  // Chance to reset spell cooldown
    float spellPowerBoost        = 0.0;    // Increases spell damage
    float manaCostReduction      = 0.0f;   // Reduces mana costs by 0%
    int   manaRegenBonus         = 50;

public:
    EtherealBond() : Skill("Ethereal Bond", 15, 6, 0, 1.0f, {}, {}, false, false) {};
};