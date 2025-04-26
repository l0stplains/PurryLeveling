#pragma once
#include "effects/Effect.hpp"

class Overclock : public Effect
{
public:
    Overclock(float hasteMul, float costMul, int defDebuff, int dur)
      : Effect("Overclock",
               "Haste×" + std::to_string(hasteMul)
             + ", Cost×" + std::to_string(costMul)
             + ", PhysDef–" + std::to_string(defDebuff),
               dur)
    {
        m_modifiers.hasteMultiplier  = hasteMul;
        m_modifiers.resourceCostMul  = costMul;
        m_modifiers.physicalDefense  = -defDebuff;
    }
};