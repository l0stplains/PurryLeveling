#pragma once
#include "effects/Effect.hpp"

class FortunesCall : public Effect
{
public:
    FortunesCall() : Effect("Fortune's Call", "Luck+5, Buff×1.3", 3)
    {
        m_modifiers.luck           = 5;
        m_modifiers.buffMultiplier = 1.3f;
    }

    FortunesCall(int luck, float buffMul, int dur)
        : Effect("Fortune’s Call",
                 "Luck+" + std::to_string(luck) + ", Buff×" + std::to_string(buffMul),
                 dur)
    {
        m_modifiers.luck           = luck;
        m_modifiers.buffMultiplier = buffMul;
    }
};