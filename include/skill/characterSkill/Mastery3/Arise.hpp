#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"

class Arise : public Skill
{
    private:
        float intelligenceMultiplier = 0.14f;
        int summonedUnit = 3;

    public:
        // Cuma iniialize stats untuk multiplier
        Arise() : Skill("Arise", 13, 6, 0, 0, {}, {}, true, true)
        {
            vector<Effect> effectVec;
            effectVec.push_back(TimeWarp(0.25f, 0.15f, 3));
            this->setEffects(effectVec);
        }
};
