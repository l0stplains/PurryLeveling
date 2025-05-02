#pragma once
#include <memory>

#include "effects/useEffects/FighterMight.hpp"
#include "skill/characterSkill/Mastery3/Howl.hpp"

// Fighter Mastery 2
class WarCry : public FighterSkill
{
public:
    WarCry(bool isLearned = false, bool isActivated = false)
        : FighterSkill("War Cry", 8, 2, 5, 1.0f, {}, {}, isLearned, isActivated, 0.20f)
    {
        vector<Skill*> thirdSkill;
        Howl*          howl = new Howl();
        thirdSkill.push_back(howl);
        this->setChildren(thirdSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FighterMight>(15, 0.5f, 0.0f, 5, 0, 0, 3));
        this->setEffects(std::move(effectVec));
    };

    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};