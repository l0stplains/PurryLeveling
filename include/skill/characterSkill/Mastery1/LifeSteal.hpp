#pragma once
#include "skill/characterSkill/Mastery2/BloodRitual.hpp"
#include "skill/characterSkill/Mastery2/UndeadLegion.hpp"

// Root Skill untuk Necromancer
class LifeSteal : public NecromancerSkill
{
public:
    LifeSteal(bool isLearned = true, bool isActivated = true)
        : NecromancerSkill("LifeSteal", 6, 2, 0, 0, {}, {}, isLearned, isActivated, 0.13f, 0)
    {
        vector<Skill*> secondSkill;
        BloodRitual*  bloodRitual  = new BloodRitual();
        UndeadLegion* undeadLegion = new UndeadLegion();
        secondSkill.push_back(bloodRitual);
        secondSkill.push_back(undeadLegion);
        this->setChildren(secondSkill);
    };

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int getSummonedUnit() const override { return summonedUnit; }
};