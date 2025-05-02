#pragma once
#include "skill/characterSkill/Mastery3/SoulHarvest.hpp"
#include "skill/characterSkill/NecromancerSkill.hpp"

// Necromancer Mastery 2
class BloodRitual : public NecromancerSkill
{
public:
    BloodRitual(bool isLearned = false, bool isActivated = false)
        : NecromancerSkill("Blood Ritual", 8, 3, 0, 0, {}, {}, isLearned, isActivated, 0.30f, 0)
    {
        vector<unique_ptr<Skill>> thirdSkill;
        thirdSkill.push_back(std::make_unique<SoulHarvest>());
        this->setChildren(std::move(thirdSkill));
    }

    float getLifestealPercentage() const override { return lifestealPercentage; }
    int   getSummonedUnit() const override { return summonedUnit; }
};