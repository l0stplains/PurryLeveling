#pragma once
#include "effects/Effect.hpp"

class Shadowstep : public Effect
{
public:
    Shadowstep(float dodge, float hasteMul, float accMul, int dur)
      : Effect("Shadowstep",
               "Dodge+" + std::to_string(int(dodge*100)) + "%"
             + ", Haste×" + std::to_string(hasteMul)
             + ", Acc×" + std::to_string(accMul),
               dur)
    {
        m_modifiers.dodgeChance   = dodge;
        m_modifiers.hasteMultiplier = hasteMul;
        m_modifiers.accuracy       = accMul;
    }

};