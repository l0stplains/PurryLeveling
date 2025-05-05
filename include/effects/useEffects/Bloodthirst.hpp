#pragma once
#include "effects/Effect.hpp"

class Bloodthirst : public Effect
{
public:
Bloodthirst()
    : Effect("Bloodthirst", "Str+5, Crit%+10%, Skip%+5%", 3)
{
    m_modifiers.strength = 5;
    m_modifiers.criticalStrikeChance = 0.1f;
    m_modifiers.skipTurnChance = 0.05f;
}
    Bloodthirst(int str, float critChance, float skipChance, int dur)
        : Effect("Bloodthirst",
                 "Str+" + std::to_string(str) + ", Crit%+" + std::to_string(int(critChance * 100)) +
                     "%" + ", Skip%+" + std::to_string(int(skipChance * 100)) + "%",
                 dur)
    {
        m_modifiers.strength             = str;
        m_modifiers.criticalStrikeChance = critChance;
        m_modifiers.skipTurnChance       = skipChance;
    }
};