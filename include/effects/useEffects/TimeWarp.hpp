#pragma once
#include "effects/Effect.hpp"

class TimeWarp : public Effect
{
public:
    TimeWarp() : Effect("Time Warp", "Haste×1.3, Res–20%", 3)
    {
        m_modifiers.hasteMultiplier  = 1.3f;
        m_modifiers.statusResistance = -0.2f;
    }

    TimeWarp(float hasteMul, float statResDebuff, int dur)
        : Effect("Time Warp",
                 "Haste×" + std::to_string(hasteMul) + ", Res–" +
                     std::to_string(int(statResDebuff * 100)) + "%",
                 dur)
    {
        m_modifiers.hasteMultiplier  = hasteMul;
        m_modifiers.statusResistance = -statResDebuff;
    }
};