#pragma once
#include "effects/Effect.hpp"

class MysticBarrier : public Effect
{
public:
    MysticBarrier() : Effect("Mystic Barrier", "MagDef+5, Res+15%, Haste×0.8", 3)
    {
        m_modifiers.magicDefense     = 5;
        m_modifiers.statusResistance = 0.15f;
        m_modifiers.hasteMultiplier  = 0.8f;
    }

    MysticBarrier(int magDef, float statRes, float hasteMul, int dur)
        : Effect("Mystic Barrier",
                 "MagDef+" + std::to_string(magDef) + ", Res+" + std::to_string(int(statRes * 100)) +
                     "%" + ", Haste×" + std::to_string(hasteMul),
                 dur)
    {
        m_modifiers.magicDefense     = magDef;
        m_modifiers.statusResistance = statRes;
        m_modifiers.hasteMultiplier  = hasteMul;
    }
};