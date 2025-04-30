#pragma once
#include "skill/Skill.hpp"

// Skill Umum dari BOSS
class Rage : public Skill
{
private:
    float rageMultiplier = 1.5f;

public:
    Rage(bool isLearned = true, bool isActivated = true)
        : Skill("Rage", 0, 0, 0, 0, {}, {}, isLearned, isActivated) {};

    float getRageMultiplier() const { return rageMultiplier; }
};