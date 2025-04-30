#pragma once
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/Skill.hpp"

class Vanish : public Skill
{
private:
    float agilityMultiplier = 0.30f;
    float chanceNewTurn     = 0.20f;

public:
    Vanish() : Skill("Vanish", 10, 5, 0, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        effectVec.push_back(Shadowstep(78, 0.45f, 0.38f, 4));
        this->setEffects(effectVec);
    }
};
