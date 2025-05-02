#pragma once
#include <memory>

#include "effects/useEffects/FighterMight.hpp"
#include "skill/characterSkill/Mastery3/Guardian.hpp"

// Fighter Master 2
class IronWill : public FighterSkill
{
public:
    IronWill(bool isLearned = false, bool isActivated = false)
        : FighterSkill("Iron Will", 7, 3, 0, 0.6f, {}, {}, isLearned, isActivated, 0.15f)
    {
        vector<Skill*> thirdSkill;
        Guardian* guardian = new Guardian();
        thirdSkill.push_back(guardian);
        this->setChildren(thirdSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FighterMight>(10, 0.05f, 0.01f, 4, 25, 15, 2));
        this->setEffects(std::move(effectVec));
    }

    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};