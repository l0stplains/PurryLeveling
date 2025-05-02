#pragma once
#include "skill/characterSkill/FighterSkill.hpp"
#include "skill/characterSkill/Mastery2/IronWill.hpp"
#include "skill/characterSkill/Mastery2/Warcry.hpp"

// Root Skill Untuk Fighter
class Bravery : public FighterSkill
{
public:
    Bravery(bool isLearned = true, bool isActivated = true)
        : FighterSkill("Bravery", 4, 0, 0, 0, {}, {}, isLearned, isActivated, 0.12f)
    {
        vector<unique_ptr<Skill>> secondSkill;
        secondSkill.push_back(std::make_unique<WarCry>());
        secondSkill.push_back(std::make_unique<IronWill>());
        this->setChildren(std::move(secondSkill));
    }

    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};
