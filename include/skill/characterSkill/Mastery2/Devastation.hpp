#pragma once
#include <memory>

#include "effects/useEffects/BattleFocus.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/characterSkill/Mastery3/Annihilation.hpp"

// Berserker Mastery 2
class Devastation : public BerserkerSkill
{
public:
    Devastation(bool isLearned = false, bool isActivated = false)
        : BerserkerSkill(
              "Devastation", 9, 4, 12, 1.0f, {}, {}, isLearned, isActivated, 0.25f, 0.0f, 0.0f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<BattleFocus>(1.25f, 0.15f, 0.10f, 2));
        effectVec.push_back(std::make_unique<HarmoniousEmpowerment>(25, 10, 0.25f, 4));
        this->setEffects(std::move(effectVec));

        vector<unique_ptr<Skill>> thirdSkill;
        thirdSkill.push_back(std::make_unique<Annihilation>());
        this->setChildren(std::move(thirdSkill));
    }

    float getFuryMultiplier() const override { return furyMultiplier; }
    float getLifeStealFactor() const override { return lifeStealFactor; }
    float getSelfDamage() const override { return selfDamage; }
};
