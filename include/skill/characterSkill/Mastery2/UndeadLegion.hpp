#pragma once
#include "skill/characterSkill/Mastery3/Arise.hpp"

// Necromancer Matery 2
class UndeadLegion : public NecromancerSkill
{
public:
    UndeadLegion(bool isLearned = false, bool isActivated = false)
        : NecromancerSkill("Undead Legion", 10, 4, 0, 0, {}, {}, isLearned, isActivated, 0.05f, 1)
    {
        vector<Skill*> thirdSkill;
        Arise* arise = new Arise();
        thirdSkill.push_back(arise);
        this->setChildren(thirdSkill);
    }

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int getSummonedUnit() const override { return summonedUnit; }
};
