#pragma once
#include "effects/Effect.hpp"

class Brittle : public Effect
{
public:
    Brittle() : Effect("Brittle", "PhysDef–4", 2) { m_modifiers.physicalDefense = -4; }

    Brittle(int defPen) : Effect("Brittle", "PhysDef–" + std::to_string(defPen), 2)
    {
        m_modifiers.physicalDefense = -defPen;
    }
};