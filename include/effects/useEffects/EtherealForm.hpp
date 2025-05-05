#pragma once
#include "effects/Effect.hpp"

class EtherealForm : public Effect
{
public:
    EtherealForm()
        : Effect("Ethereal Form", "Dodge+20%, Res+15%, PhysDef–3, MagDef–3", 2)
    {
        m_modifiers.dodgeChance = 0.2f;
        m_modifiers.statusResistance = 0.15f;
        m_modifiers.physicalDefense = -3;
        m_modifiers.magicDefense = -3;
    }

    EtherealForm(float dodgeAdd, float statResAdd, int defPen, int dur)
        : Effect("Ethereal Form",
                 "Dodge+" + std::to_string(int(dodgeAdd * 100)) + "%" + ", Res+" +
                     std::to_string(int(statResAdd * 100)) + "%" + ", PhysDef–" +
                     std::to_string(defPen) + ", MagDef–" + std::to_string(defPen),
                 dur)
    {
        m_modifiers.dodgeChance      = dodgeAdd;
        m_modifiers.statusResistance = statResAdd;
        m_modifiers.physicalDefense  = -defPen;
        m_modifiers.magicDefense     = -defPen;
    }
};