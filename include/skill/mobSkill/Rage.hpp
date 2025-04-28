#pragma once
#include "skill/Skill.hpp"

// Skill Umum dari BOSS
class Rage : public Skill
{
    private:
        float rageMultiplier = 1.5f;

    public:
        Rage() : Skill("Rage", 0, 0, 0, 0, {}, {}, true, true){};

        float getRageMultiplier() const { return rageMultiplier; }
};