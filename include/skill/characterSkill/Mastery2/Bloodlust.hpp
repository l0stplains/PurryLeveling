#pragma once
#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/characterSkill/BerserkerSkill.hpp"

// Berserker Mastery 2
class Bloodlust : public BerserkerSkill
{
public:
    Bloodlust(bool isLearned = false, bool isActivated = false)
        : BerserkerSkill("Bloodlust", 7, 2, 6, 1.0f, {}, {}, isLearned, isActivated, 0.18f, 0.15f, 0.0f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Bloodthirst>(5, 0.10f, 0.05f, 3));
        effectVec.push_back(make_unique<HarmoniousEmpowerment>(15, 5, 0.18f, 4));
        this->setEffects(std::move(effectVec));
    }

    float getFuryMultiplier() const override { return furyMultiplier; }
    float getLifeStealFactor() const override { return lifeStealFactor; }
    float getSelfDamage() const override { return selfDamage; }
};