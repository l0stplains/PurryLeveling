#pragma once
#include "effects/Effect.hpp"

class SilenceEffect : public Effect
{
public:
    SilenceEffect(int intPen, float accMul, int dur)
      : Effect("Silence",
               "Int–" + std::to_string(intPen)
             + ", Acc×" + std::to_string(accMul),
               dur)
    {
        m_modifiers.intelligence = -intPen;
        m_modifiers.accuracy     = accMul;
    }
};