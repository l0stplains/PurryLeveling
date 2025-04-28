#pragma once
#include "effects/Effect.hpp"

class FortunesCall : public Effect
{
public:
    FortunesCall(int luck, float buffMul, int dur)
        : Effect("Fortune’s Call",
                 "Luck+" + std::to_string(luck) + ", Buff×" + std::to_string(buffMul),
                 dur)
    {
        m_modifiers.luck           = luck;
        m_modifiers.buffMultiplier = buffMul;
    }
};