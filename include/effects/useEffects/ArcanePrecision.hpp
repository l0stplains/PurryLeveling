#pragma once
#include "effects/Effect.hpp"

class ArcanePrecision : public Effect
{
public:
    ArcanePrecision(int intel, float accMul, float critMul, int dur)
      : Effect("Arcane Precision",
               "Int+" + std::to_string(intel)
             + ", Acc×" + std::to_string(accMul)
             + ", Crit×" + std::to_string(critMul),
               dur)
    {
        m_modifiers.intelligence             = intel;
        m_modifiers.accuracy                 = accMul;
        m_modifiers.criticalStrikeMultiplier = critMul;
    }
};
