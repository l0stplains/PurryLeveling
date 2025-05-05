#pragma once
#include "effects/Effect.hpp"

class FighterMight : public Effect
{
public:
    FighterMight()
        : Effect("Fighter's Might", "Str+5, Crit×1.3, Crit%10, Luck+2, PhysDef+3, MagDef+2", 3)
    {
        m_modifiers.strength                 = 5;
        m_modifiers.criticalStrikeMultiplier = 1.3f;
        m_modifiers.criticalStrikeChance     = 0.1f;
        m_modifiers.luck                     = 2;
        m_modifiers.physicalDefense          = 3;
        m_modifiers.magicDefense             = 2;
    }

    FighterMight(int   strBonus,
                 float critMultBonus,
                 float critChanceBonus,
                 int   luckBonus,
                 int   physDefBonus,
                 int   magDefBonus,
                 int   duration)
        : Effect("Fighter's Might",
                 "Str+" + std::to_string(strBonus) + ", Crit×" + std::to_string(critMultBonus) +
                     ", Crit%" + std::to_string(int(critChanceBonus * 100)) + ", Luck+" +
                     std::to_string(luckBonus) + ", PhysDef+" + std::to_string(physDefBonus) +
                     ", MagDef+" + std::to_string(magDefBonus),
                 duration)
    {
        m_modifiers.strength                 = strBonus;
        m_modifiers.criticalStrikeMultiplier = critMultBonus;
        m_modifiers.criticalStrikeChance     = critChanceBonus;
        m_modifiers.luck                     = luckBonus;
        m_modifiers.physicalDefense          = physDefBonus;
        m_modifiers.magicDefense             = magDefBonus;
    }
};