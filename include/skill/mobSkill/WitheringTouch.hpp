#pragma once
#include "skill/Skill.hpp"

class WitheringTouch : public Skill
{
private:
    float intMultiplier = 0.69f;
    float manaReduction = intMultiplier * 0.15f;
    int   duration      = 4;  // turn

public:
    WitheringTouch(bool isLearned = false, bool isActivated = false) 
        : Skill("Withering Touch", 0, 0, 0, 0.0f, {}, {}, isLearned, isActivated) {};

    float getIntMultiplier() const { return intMultiplier; }
    float getManaReduction() const { return manaReduction; }
    int   getDuration() const { return duration; }
    void  setIntMultiplier(float multiplier) { intMultiplier = multiplier; }
};