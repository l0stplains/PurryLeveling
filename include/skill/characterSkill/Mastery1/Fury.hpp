#pragma once
#include <memory>

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
        vector<Skill*> secondSkill;
        Bloodlust*     bloodlust   = new Bloodlust();
        Devastation*   devastation = new Devastation();
        secondSkill.push_back(bloodlust);
        secondSkill.push_back(devastation);
        this->setChildren(secondSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<HarmoniousEmpowerment>(0, 0, 0.125f, 3));
    };

    float getFuryMultiplier() const override { return furyMultiplier; }
    float getLifeStealFactor() const override { return lifeStealFactor; }
    float getSelfDamage() const override { return selfDamage; }
};