#pragma once
#include "skill/characterSkill/Mastery3/Arise.hpp"

// Necromancer Mastery 2
class UndeadLegion : public NecromancerSkill
{
public:
    UndeadLegion(bool isLearned = false, bool isActivated = false)
        : NecromancerSkill("Undead Legion", 10, 4, 0, 0, {}, {}, isLearned, isActivated, 0.05f, 1)
    {
        vector<unique_ptr<Skill>> thirdSkill;
        thirdSkill.push_back(std::make_unique<Arise>());
        this->setChildren(std::move(thirdSkill));
    }

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int   getSummonedUnit() const override { return summonedUnit; }
};
