#pragma once
#include "effects/Effect.hpp"

class CurseOfFragilityEffect : public Effect
{
public:
    CurseOfFragilityEffect()
        : Effect("Curse of Fragility", "PhysDef–4, MagDef–4, Buff×0.8", 3)
    {
        m_modifiers.physicalDefense = -4;
        m_modifiers.magicDefense = -4;
        m_modifiers.buffMultiplier = 0.8f;
    }

    CurseOfFragilityEffect(int defPen, float buffMulDebuff, int dur)
        : Effect("Curse of Fragility",
                 "PhysDef–" + std::to_string(defPen) + ", MagDef–" + std::to_string(defPen) +
                     ", Buff×" + std::to_string(buffMulDebuff),
                 dur)
    {
        m_modifiers.physicalDefense = -defPen;
        m_modifiers.magicDefense    = -defPen;
        m_modifiers.buffMultiplier  = buffMulDebuff;
    }
};