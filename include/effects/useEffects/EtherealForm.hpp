#pragma once
#include "effects/Effect.hpp"

class EtherealForm : public Effect
{
public:
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