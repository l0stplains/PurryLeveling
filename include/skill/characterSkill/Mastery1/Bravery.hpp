#pragma once
#include <memory>

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
        vector<Skill*> secondSkill;
        WarCry*   warCry   = new WarCry();
        IronWill* ironWill = new IronWill();
        secondSkill.push_back(warCry);
        secondSkill.push_back(ironWill);
        this->setChildren(secondSkill);
    };

    float getStrengthMultiplier() const override { return this->strengthMultiplier; }
};