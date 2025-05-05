#pragma once
#include "effects/Effect.hpp"

class Shadowstep : public Effect
{
public:
    Shadowstep()
        : Effect("Shadowstep", "Dodge+15%, Haste×1.2, Acc×0.9", 2)
    {
        m_modifiers.dodgeChance = 0.15f;
        m_modifiers.hasteMultiplier = 1.2f;
        m_modifiers.accuracy = 0.9f;
    }

    Shadowstep(float dodge, float hasteMul, float accMul, int dur)
        : Effect("Shadowstep",
                 "Dodge+" + std::to_string(int(dodge * 100)) + "%" + ", Haste×" +
                     std::to_string(hasteMul) + ", Acc×" + std::to_string(accMul),
                 dur)
    {
        m_modifiers.dodgeChance     = dodge;
        m_modifiers.hasteMultiplier = hasteMul;
        m_modifiers.accuracy        = accMul;
    }
};