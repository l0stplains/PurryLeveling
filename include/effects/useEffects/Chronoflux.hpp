#pragma once
#include "effects/Effect.hpp"

class Chronoflux : public Effect
{
public:
    Chronoflux() : Effect("Chrono Mastery", "Resource cost×0.7, Haste×1.3", 3)
    {
        m_modifiers.resourceCostMul = 0.7f;
        m_modifiers.hasteMultiplier = 1.3f;
    }

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