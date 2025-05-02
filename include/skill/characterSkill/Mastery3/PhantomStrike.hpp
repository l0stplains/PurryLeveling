#pragma once
#include <memory>

#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/characterSkill/AssassinSkill.hpp"
using namespace std;

// Skill Mastery3 Assassin
class PhantomStrike : public AssassinSkill
{
public:
    PhantomStrike(bool isLearned = false, bool isActive = false)
        : AssassinSkill("Phantom Strike", 14, 6, 25, 0.64f, {}, {}, isLearned, isActive, 0.25f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<TimeWarp>(0.40f, 0.131f, 3));
        effectVec.push_back(make_unique<Bloodthirst>(40, 0.35, 0.15, 3));
        this->setEffects(std::move(effectVec));
    }

    float getAgilityMultiplier() const override { return this->agilityMultiplier; }
};
