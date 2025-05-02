#pragma once
#include <memory>

#include "skill/characterSkill/FighterSkill.hpp"
#include "skill/characterSkill/Mastery2/IronWill.hpp"
#include "skill/characterSkill/Mastery2/Warcry.hpp"

// Root Skill Untuk Fighter
class Bravery : public Skill
{
private:
    float strengthMultiplier = 0.12f;  // Untuk heal

public:
    Bravery(bool isLearned = true, bool isActivated = true)
        : Skill("Bravery", 4, 0, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> secondSkill;

        WarCry*   warCry   = new WarCry();
        IronWill* ironWill = new IronWill();
        secondSkill.push_back(warCry);
        secondSkill.push_back(ironWill);
        this->setChildren(secondSkill);
    };

    float getStrengthMultiplier() override const { return strengthMultiplier; }
};