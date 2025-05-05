#pragma once
#include "effects/Effect.hpp"

class Overclock : public Effect
{
public:
    Overclock()
        : Effect("Overclock", "Haste×1.5, Cost×1.2, PhysDef–3", 2)
    {
        m_modifiers.hasteMultiplier = 1.5f;
        m_modifiers.resourceCostMul = 1.2f;
        m_modifiers.physicalDefense = -3;
    }

    Overclock(float hasteMul, float costMul, int defDebuff, int dur)
        : Effect("Overclock",
                 "Haste×" + std::to_string(hasteMul) + ", Cost×" + std::to_string(costMul) +
                     ", PhysDef–" + std::to_string(defDebuff),
                 dur)
    {
        m_modifiers.hasteMultiplier = hasteMul;
        m_modifiers.resourceCostMul = costMul;
        m_modifiers.physicalDefense = -defDebuff;
    }
};