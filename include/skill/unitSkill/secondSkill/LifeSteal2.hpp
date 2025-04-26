#pragma once
#include "skill/Skill.hpp"
#include "skill/unitSkill/thirdSkill/LifeSteal3.hpp"

class BloodRitual : public Skill
{
    private:
        float damageMultiplier = 0.17f;
        int summonedUnit = 0;

    public:
        BloodRitual() : Skill("Blood Ritual", 8, 2, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> thirdSkill;

            SoulHarvest* soulHarvest = new SoulHarvest();
            thirdSkill.push_back(soulHarvest);
            this->setChildren(thirdSkill);
        };
};

class UndeadLegion : public Skill
{
    private:
        float damageMultiplier = 0.12f;
        int summonedUnit = 1;

    public:
        UndeadLegion() : Skill("Undead Legion", 10, 3, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> thirdSkill;

            SoulPact* soulPact = new SoulPact();
            thirdSkill.push_back(soulPact);
            this->setChildren(thirdSkill);
        };
};