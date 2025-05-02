#pragma once
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "effects/useEffects/Overclock.hpp"
#include "skill/characterSkill/BerserkerSkill.hpp"

// Berserkser Master3 Skill
class Annihilation : public BerserkerSkill
{
public:
    Annihilation(bool isLearned = false, bool isActive = false)
        : BerserkerSkill(
              "Annihilation", 15, 6, 25, 1.0f, {}, {}, isLearned, isActive, 0.04f, 0.0f, 0.10f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<GladiatorMight>(10, 5, 1.2f, 3));
        effectVec.push_back(make_unique<Overclock>(1.5f, 1.3f, 10, 2));
        effectVec.push_back(make_unique<HarmoniousEmpowerment>(10, 5, furyMultiplier, 3));
        this->setEffects(std::move(effectVec));
    }

    float getFuryMultiplier() const override { return this->furyMultiplier; }
    float getLifeStealFactor() const override { return lifeStealFactor; }
    float getSelfDamage() const override { return selfDamage; }
};
