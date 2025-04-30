#pragma once
#include "effects/Effect.hpp"

class HarmoniousEmpowerment : public Effect
{
public:
    HarmoniousEmpowerment(int primaryStatBonus, int defenseBonus, float buffMultBonus, int duration)
        : Effect("Harmonious Empowerment",
                 "All Primary+" + std::to_string(primaryStatBonus) + ", Defenses+" +
                     std::to_string(defenseBonus) + ", BuffMul×" + std::to_string(buffMultBonus),
                 duration)
    {
        // Primary stats
        m_modifiers.strength     = primaryStatBonus;
        m_modifiers.intelligence = primaryStatBonus;
        m_modifiers.agility      = primaryStatBonus;

        // Defenses
        m_modifiers.physicalDefense = defenseBonus;
        m_modifiers.magicDefense    = defenseBonus;

        // Enhanced buff multiplier (the main focus)
        m_modifiers.buffMultiplier = buffMultBonus;
    }
};