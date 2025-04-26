#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"

class SoulHarvest : public Skill
{
    private:
        float intelligenceMultiplier = 0.25f;
        int summonedUnit = 1;

    public:
        // Cuma iniialize stats untuk multiplier
        SoulHarvest() : Skill("Soul Harvest", 12, 5, 0, 0, {}, {}, true, true){
            vector<Effect> effectVec;
            effectVec.push_back(Shadowstep(20.0f, 0.25f, 0.15f, 3));
            this->setEffects(effectVec);
        };
};

class SoulPact : public Skill
{
    private:
        float intelligenceMultiplier = 0.14f;
        int summonedUnit = 3;

    public:
        // Cuma iniialize stats untuk multiplier
        SoulPact() : Skill("Soul Pact", 13, 6, 0, 0, {}, {}, true, true)
        {
            vector<Effect> effectVec;
            effectVec.push_back(TimeWarp(0.25f, 0.15f, 3));
            this->setEffects(effectVec);
        };
};