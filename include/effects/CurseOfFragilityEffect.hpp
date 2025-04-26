#pragma once
#include "effects/Effect.hpp"

class CurseOfFragilityEffect : public Effect
{
public:
    CurseOfFragilityEffect(int defPen, float buffMulDebuff, int dur)
      : Effect("Curse of Fragility",
               "PhysDef–" + std::to_string(defPen)
             + ", MagDef–" + std::to_string(defPen)
             + ", Buff×" + std::to_string(buffMulDebuff),
               dur)
    {
        m_modifiers.physicalDefense  = -defPen;
        m_modifiers.magicDefense     = -defPen;
        m_modifiers.buffMultiplier   = buffMulDebuff;
    }
};