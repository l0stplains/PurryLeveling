#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Arise.hpp"

// Necromancer Matery 2
class UndeadLegion : public Skill
{
private:
    float damageMultiplier = 0.12f;
    int   summonedUnit     = 1;

public:
    UndeadLegion(bool isLearned = false, bool isActivated = false) 
        : Skill("Undead Legion", 10, 4, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;

        Arise* arise = new Arise();
        thirdSkill.push_back(arise);
        this->setChildren(thirdSkill);
    }
    
    float getDamageMultiplier() const { return damageMultiplier; }
    int getSummonedUnit() const { return summonedUnit; }
};
