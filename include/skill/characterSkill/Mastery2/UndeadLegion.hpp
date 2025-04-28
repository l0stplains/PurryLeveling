#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Arise.hpp"

class UndeadLegion : public Skill
{
    private:
        float damageMultiplier = 0.12f;
        int summonedUnit = 1;

    public:
        UndeadLegion() : Skill("Undead Legion", 10, 3, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> thirdSkill;

            Arise* arise = new Arise();
            thirdSkill.push_back(arise);
            this->setChildren(thirdSkill);
        }
};
