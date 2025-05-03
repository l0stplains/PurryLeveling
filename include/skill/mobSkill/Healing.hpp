#pragma once
#include "skill/Skill.hpp"

// Skill Umum dari Mob
class Healing : public Skill
{
private:
    float basicHealValue = 0.2f;

public:
    Healing(bool isLearned = true, bool isActivated = true)
        : Skill("Healing", 10, 0, 0, 0, {}, {}, isLearned, isActivated) {};

    float getBasicHealValue() const { return basicHealValue; }
};