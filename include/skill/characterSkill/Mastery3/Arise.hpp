#pragma once
#include <memory>
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/characterSkill/NecromancerSkill.hpp"

// Skill Mastery3 Necromancer
class Arise : public NecromancerSkill
{
public:
    Arise(bool isLearned = false, bool isActive = false)
        : NecromancerSkill("Arise", 13, 6, 0, 0, {}, {}, isLearned, isActive, 0.14f, 3)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<TimeWarp>(0.25f, 0.15f, 3));
        this->setEffects(std::move(effectVec));
    }

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int getSummonedUnit() const override { return summonedUnit; }
};
