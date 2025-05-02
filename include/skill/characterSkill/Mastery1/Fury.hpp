#pragma once
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/characterSkill/Mastery2/Bloodlust.hpp"
#include "skill/characterSkill/Mastery2/Devastation.hpp"

// Root Skill untuk Berserker
class Fury : public BerserkerSkill
{
public:
    Fury(bool isLearned = true, bool isActivated = true)
        : BerserkerSkill("Fury", 5, 0, 0, 0, {}, {}, isLearned, isActivated, 0.125f, 0.0f, 0.0f)
    {
        vector<unique_ptr<Skill>> secondSkill;
        secondSkill.push_back(std::make_unique<Bloodlust>());
        secondSkill.push_back(std::make_unique<Devastation>());
        this->setChildren(std::move(secondSkill));

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<HarmoniousEmpowerment>(0, 0, 0.125f, 3));
        this->setEffects(std::move(effectVec));
    }

    float getFuryMultiplier() const override { return furyMultiplier; }
    float getLifeStealFactor() const override { return lifeStealFactor; }
    float getSelfDamage() const override { return selfDamage; }
};
