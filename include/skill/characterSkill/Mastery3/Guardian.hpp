#pragma once
#include <memory>

#include "effects/useEffects/FighterMight.hpp"
#include "skill/characterSkill/FighterSkill.hpp"

// Skill Mastery3 Fighter
class Guardian : public FighterSkill
{
public:
    Guardian(bool isLearned = false, bool isActive = false)
        : FighterSkill("Guardian", 15, 6, 5, 1.0f, {}, {}, isLearned, isActive, 0.20f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FighterMight>(15, 0.50f, 0.25f, 8, 40, 35, 4));
        this->setEffects(std::move(effectVec));
    }

    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};