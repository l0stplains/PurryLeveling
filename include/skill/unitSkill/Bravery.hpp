#pragma once
#include "skill/Skill.hpp"
#include "skill/unitSkill/secondSkill/Bravery2.hpp"

class Bravery : public Skill
{
    private:
        float strengthMultiplier = 0.12f;           
        float damageBoost = 0.0f;           // Additional damage boost
        float damageReduction = 0.0f; 
        float critChanceBonus = 0.0f;       // Increased critical hit chance
        float healthCost = 0.0f;            // Costs health to maintain
        float reflectDamage = 0.0f;  

    public:
        Bravery() : Skill("Bravery", 4, 0, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> secondSkill;

            WarCry* warCry = new WarCry();
            IronWill* ironWill = new IronWill();
            secondSkill.push_back(warCry);
            secondSkill.push_back(ironWill);
            this->setChildren(secondSkill);
        };
};