#pragma once
#include "effects/Effect.hpp"

class BattleFocus : public Effect
{
public:
    BattleFocus() : Effect("Battle Focus", "Acc×1.2, Crit%+5%, Dodge–5%", 3)
    {
        m_modifiers.accuracy             = 1.2;
        m_modifiers.criticalStrikeChance = 0.05;
        m_modifiers.dodgeChance          = -0.05;
    }
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