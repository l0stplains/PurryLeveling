#pragma once
#include "effects/Effect.hpp"

class GladiatorMight : public Effect
{
public:
    GladiatorMight() : Effect("Gladiator's Might", "Str+6, PhysDef+4, Cost×1.1", 3)
    {
        m_modifiers.strength        = 6;
        m_modifiers.physicalDefense = 4;
        m_modifiers.resourceCostMul = 1.1f;
    }

    GladiatorMight(int str, int physDef, float costMul, int dur)
        : Effect("Gladiator’s Might",
                 "Str+" + std::to_string(str) + ", PhysDef+" + std::to_string(physDef) + ", Cost×" +
                     std::to_string(costMul),
                 dur)
    {
        m_modifiers.strength        = str;
        m_modifiers.physicalDefense = physDef;
        m_modifiers.resourceCostMul = costMul;
    }
};