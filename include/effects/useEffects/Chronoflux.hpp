#pragma once
#include "effects/Effect.hpp"

class Chronoflux : public Effect
{
public:
    Chronoflux(float resourceReduction, float hasteBonus, int duration)
        : Effect("Chrono Mastery",
                 "Resource cost×" + std::to_string(resourceReduction) + ", Haste×" +
                     std::to_string(hasteBonus),
                 duration)
    {
        m_modifiers.resourceCostMul = resourceReduction;
        m_modifiers.hasteMultiplier = hasteBonus;
    }
};