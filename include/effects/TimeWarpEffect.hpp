#pragma once
#include "effects/Effect.hpp"

class TimeWarpEffect : public Effect
{
public:
    TimeWarpEffect(float hasteMul, float statResDebuff, int dur)
      : Effect("Time Warp",
               "Haste×" + std::to_string(hasteMul)
             + ", Res–" + std::to_string(int(statResDebuff*100)) + "%",
               dur)
    {
        m_modifiers.hasteMultiplier  = hasteMul;
        m_modifiers.statusResistance = -statResDebuff;
    }
};