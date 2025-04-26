#pragma once
#include "effects/Effect.hpp"

class GladiatorMightEffect : public Effect
{
public:
    GladiatorMightEffect(int str, int physDef, float costMul, int dur)
      : Effect("Gladiator’s Might",
               "Str+" + std::to_string(str)
             + ", PhysDef+" + std::to_string(physDef)
             + ", Cost×" + std::to_string(costMul),
               dur)
    {
        m_modifiers.strength        = str;
        m_modifiers.physicalDefense = physDef;
        m_modifiers.resourceCostMul = costMul;
    }
};