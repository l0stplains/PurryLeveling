#pragma once
#include "effects/Effect.hpp"

class StalwartResolveEffect : public Effect
{
public:
    StalwartResolveEffect(int physDef, int magDef, int luckDebuff, int dur)
      : Effect("Stalwart Resolve",
               "PhysDef+" + std::to_string(physDef)
             + ", MagDef+" + std::to_string(magDef)
             + ", Luck–" + std::to_string(luckDebuff),
               dur)
    {
        m_modifiers.physicalDefense = physDef;
        m_modifiers.magicDefense    = magDef;
        m_modifiers.luck            = -luckDebuff;
    }
};