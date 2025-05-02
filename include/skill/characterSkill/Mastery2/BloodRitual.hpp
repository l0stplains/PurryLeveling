#pragma once
#include "skill/characterSkill/NecromancerSkill.hpp"
#include "skill/characterSkill/Mastery3/SoulHarvest.hpp"

// Necromancer Mster 2
class BloodRitual : public NecromancerSkill
{
public:
    BloodRitual(bool isLearned = false, bool isActivated = false)
        : NecromancerSkill("Blood Ritual", 8, 3, 0, 0, {}, {}, isLearned, isActivated, 0.30f, 0)
    {
        vector<Skill*> thirdSkill;

        SoulHarvest* soulHarvest = new SoulHarvest();
        thirdSkill.push_back(soulHarvest);
        this->setChildren(thirdSkill);
    }

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int getSummonedUnit() const override { return summonedUnit; }
};
