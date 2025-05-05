#pragma once
#include "effects/Effect.hpp"

class ArcanePrecision : public Effect
{
public:
    ArcanePrecision()
        : Effect("Arcane Precision", "Int+5, Acc×1.2, Crit×1.5", 3)
    {
        m_modifiers.intelligence = 5;
        m_modifiers.accuracy = 1.2f;
        m_modifiers.criticalStrikeMultiplier = 1.5f;
    }
    
    ArcanePrecision(int intel, float accMul, float critMul, int dur)
        : Effect("Arcane Precision",
                 "Int+" + std::to_string(intel) + ", Acc×" + std::to_string(accMul) + ", Crit×" +
                     std::to_string(critMul),
                 dur)
    {
        m_modifiers.intelligence             = intel;
        m_modifiers.accuracy                 = accMul;
        m_modifiers.criticalStrikeMultiplier = critMul;
    }
};
