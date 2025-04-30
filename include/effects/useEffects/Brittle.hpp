#pragma once
#include "effects/Effect.hpp"

class Brittle : public Effect
{
public:
    Brittle(int defPen) : Effect("Brittle", "PhysDef–" + std::to_string(defPen), 2)
    {
        m_modifiers.physicalDefense = -defPen;
    }
};