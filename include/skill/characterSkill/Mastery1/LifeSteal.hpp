#pragma once
#include <memory>

#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/BloodRitual.hpp"
#include "skill/characterSkill/Mastery2/UndeadLegion.hpp"

// Root Skill untuk Necromancer
class LifeSteal : public Skill
{
private:
    float lifestealPercentage = 0.13f;
    int   summonedUnit        = 0;

public:
    LifeSteal(bool isLearned = true, bool isActivated = true)
        : Skill("LifeSteal", 6, 2, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> secondSkill;

        BloodRitual*  bloodRitual  = new BloodRitual();
        UndeadLegion* undeadLegion = new UndeadLegion();
        secondSkill.push_back(bloodRitual);
        secondSkill.push_back(undeadLegion);
        this->setChildren(secondSkill);
    };

    float getLifestealPercentage() const { return lifestealPercentage; }
    int   getSummonedUnit() const { return summonedUnit; }
};