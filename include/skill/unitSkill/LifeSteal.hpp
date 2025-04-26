#pragma once
#include "skill/Skill.hpp"
#include "skill/unitSkill/secondSkill/LifeSteal2.hpp"

class LifeSteal : public Skill
{
    private:
        float lifestealPercentage = 0.13f;
        int summonedUnit = 0;

    public:
        // Cuma iniialize stats untuk multiplier
        LifeSteal() : Skill("LifeSteal", 6, 2, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> secondSkill;

            BloodRitual* bloodRitual = new BloodRitual();
            UndeadLegion* undeadLegion = new UndeadLegion();
            secondSkill.push_back(bloodRitual);
            secondSkill.push_back(undeadLegion);
            this->setChildren(secondSkill);
        };
};