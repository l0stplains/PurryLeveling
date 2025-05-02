#pragma once
#include <memory>

#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/characterSkill/NecromancerSkill.hpp"

// Skill Mastery3 Necromancer
class SoulHarvest : public NecromancerSkill
{

public:
    SoulHarvest(bool isLearned = false, bool isActive = false)
        : NecromancerSkill("Soul Harvest", 12, 5, 0, 0, {}, {}, isLearned, isActive, 0.4f, 1)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Shadowstep>(0.2f, 0.25f, 0.15f, 3));
        this->setEffects(std::move(effectVec));
    }

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int getSummonedUnit() const override { return summonedUnit; }
};
