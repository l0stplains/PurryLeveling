#pragma once
#include "effects/Effect.hpp"

class FighterMight : public Effect
{
public:
    FighterMight(int strBonus, float critMultBonus, float critChanceBonus, int luckBonus, int physDefBonus, int magDefBonus, int duration)
        : Effect("Fighter's Migght",
                "Str+" + std::to_string(strBonus) + 
                ", Crit×" + std::to_string(critMultBonus) +
                ", Crit%" + std::to_string(int(critChanceBonus * 100)) +
                ", Luck+" + std::to_string(luckBonus) +
                ", PhysDef+" + std::to_string(physDefBonus) +
                ", MagDef+" + std::to_string(magDefBonus),
                duration)
    {
        m_modifiers.strength = strBonus;
        m_modifiers.criticalStrikeMultiplier = critMultBonus;
        m_modifiers.criticalStrikeChance = critChanceBonus;
        m_modifiers.luck = luckBonus;
        m_modifiers.physicalDefense = physDefBonus;
        m_modifiers.magicDefense = magDefBonus;
    }
};