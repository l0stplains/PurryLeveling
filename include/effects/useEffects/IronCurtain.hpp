#pragma once
#include "effects/Effect.hpp"

class IronCurtain : public Effect
{
public:
    IronCurtain(int physDef, int magDef, float hasteDebuff, int dur)
        : Effect("Iron Curtain",
                 "PhysDef+" + std::to_string(physDef) + ", MagDef+" + std::to_string(magDef) +
                     ", Haste×" + std::to_string(hasteDebuff),
                 dur)
    {
        m_modifiers.physicalDefense = physDef;
        m_modifiers.magicDefense    = magDef;
        m_modifiers.hasteMultiplier = hasteDebuff;
    }
};