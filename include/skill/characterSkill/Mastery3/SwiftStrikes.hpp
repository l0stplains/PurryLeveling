#pragma once
#include <memory>

#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "skill/characterSkill/AssassinSkill.hpp"

// Skill Mastery3 Assassin
class SwiftStrikes : public AssassinSkill
{
public:
    SwiftStrikes(bool isLearned = false, bool isActive = false)
        : AssassinSkill("Swift Strikes", 12, 5, 8, 0.56f, {}, {}, isLearned, isActive, 0.25f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<GladiatorMight>(40, 13, 0.091f, 4));
        effectVec.push_back(make_unique<Bloodthirst>(20, 0.4, 0.1, 2));
        this->setEffects(std::move(effectVec));
    }

    float getAgilityMultiplier() const override { return this->agilityMultiplier; }
};
