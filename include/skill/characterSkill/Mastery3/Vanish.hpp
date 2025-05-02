#pragma once
#include <memory>

#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "skill/characterSkill/AssassinSkill.hpp"
using namespace std;

// Skill Mastery3 Assassin
class Vanish : public AssassinSkill
{
public:
    Vanish(bool isLearned = false, bool isActive = false)
        : AssassinSkill("Vanish", 10, 5, 0, 0.7f, {}, {}, isLearned, isActive, 0.30f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Shadowstep>(78, 0.45f, 0.38f, 4));
        effectVec.push_back(make_unique<Bloodthirst>(5, 0.55, 0.2, 2));
        this->setEffects(std::move(effectVec));
    }

    float getAgilityMultiplier() const override { return this->agilityMultiplier; }
};
