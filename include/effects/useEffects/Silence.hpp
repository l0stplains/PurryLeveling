#pragma once
#include "effects/Effect.hpp"

class Silence : public Effect
{
public:
    Silence() : Effect("Silence", "Int–5, Acc×0.8", 2)
    {
        m_modifiers.intelligence = -5;
        m_modifiers.accuracy     = 0.8f;
    }

    Silence(int intPen, float accMul, int dur)
        : Effect("Silence", "Int–" + std::to_string(intPen) + ", Acc×" + std::to_string(accMul), dur)
    {
        m_modifiers.intelligence = -intPen;
        m_modifiers.accuracy     = accMul;
    }
};