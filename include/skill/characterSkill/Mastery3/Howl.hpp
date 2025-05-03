#pragma once
#include <memory>

#include "effects/useEffects/FighterMight.hpp"
#include "effects/useEffects/IronCurtain.hpp"
#include "skill/characterSkill/FighterSkill.hpp"

// Skill Mastery3 Fighter
class Howl : public FighterSkill
{
public:
    Howl(bool isLearned = false, bool isActive = false)
        : FighterSkill("Howl", 12, 5, 15, 1.0f, {}, {}, isLearned, isActive, 0.35f)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<IronCurtain>(30.0f, 25.0f, 0.20f, 5));
        effectVec.push_back(make_unique<FighterMight>(25, 0.5f, 0.25f, 6, 30, 20, 4));
        this->setEffects(std::move(effectVec));
    }
    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};