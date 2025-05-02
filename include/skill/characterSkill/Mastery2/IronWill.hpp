#pragma once
#include "effects/useEffects/FighterMight.hpp"
#include "skill/characterSkill/Mastery3/Guardian.hpp"

// Fighter Mastery 2
class IronWill : public FighterSkill
{
public:
    IronWill(bool isLearned = false, bool isActivated = false)
        : FighterSkill("Iron Will", 7, 3, 0, 0.6f, {}, {}, isLearned, isActivated, 0.15f)
    {
        vector<unique_ptr<Skill>> thirdSkill;
        thirdSkill.push_back(std::make_unique<Guardian>());
        this->setChildren(std::move(thirdSkill));

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<FighterMight>(10, 0.05f, 0.01f, 4, 25, 15, 2));
        this->setEffects(std::move(effectVec));
    }

    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};
