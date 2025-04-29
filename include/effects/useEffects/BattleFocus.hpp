#pragma once
#include "effects/Effect.hpp"

class BattleFocus : public Effect
{
public:
    BattleFocus(float accMul, float critAdd, float dodgeDebuff, int dur)
        : Effect("Battle Focus",
                 "Acc×" + std::to_string(accMul) + ", Crit%+" + std::to_string(int(critAdd * 100)) +
                     "%" + ", Dodge–" + std::to_string(int(dodgeDebuff * 100)) + "%",
                 dur)
    {
        m_modifiers.accuracy             = accMul;
        m_modifiers.criticalStrikeChance = critAdd;
        m_modifiers.dodgeChance          = -dodgeDebuff;
    }
};