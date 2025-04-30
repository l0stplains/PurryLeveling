#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/SoulHarvest.hpp"

// Necromancer Mster 2
class BloodRitual : public Skill
{
private:
    float damageMultiplier = 0.17f;
    int   summonedUnit     = 0;

public:
    BloodRitual(bool isLearned = false, bool isActivated = false) 
        : Skill("Blood Ritual", 8, 3, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;

        SoulHarvest* soulHarvest = new SoulHarvest();
        thirdSkill.push_back(soulHarvest);
        this->setChildren(thirdSkill);
    }
    
    float getDamageMultiplier() const { return damageMultiplier; }
    int getSummonedUnit() const { return summonedUnit; }
};
