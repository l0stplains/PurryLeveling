#pragma once
#include "skill/Skill.hpp"
#include "skill/unitSkill/thirdSkill/Bravery3.hpp"

class WarCry : public Skill
{
    private:  
        float strengthMultiplier = 0.20f;    // Enhanced strength boost
        float damageBoost = 0.15f;           // Additional damage boost
        float damageReduction = 0.0f; 
        float critChanceBonus = 0.0f;       // Increased critical hit chance
        float healthCost = 0.0f;            // Costs health to maintain
        float reflectDamage = 0.0f; 

    public:
        WarCry() : Skill("War Cry", 8, 2, 5, 1.0f, {}, {}, false, false)
        {
            vector<Skill*> thirdSkill;

            Berserker* berserker = new Berserker();
            thirdSkill.push_back(berserker);
            this->setChildren(thirdSkill);
        };
};

class IronWill : public Skill
{
    private:
        float strengthMultiplier = 0.15f;    // Modest strength boost
        float damageReduction = 0.20f;       // Damage reduction
        float damageBoost = 0.0f;  
        float critChanceBonus = 0.0f;       // Increased critical hit chance
        float healthCost = 0.0f;            // Costs health to maintain
        float reflectDamage = 0.0f; 

    public:
        IronWill() : Skill("Iron Will", 7, 3, 0, 1.0f, {}, {}, false, false)
        {
            vector<Skill*> thirdSkill;

            Guardian* guardian = new Guardian();
            thirdSkill.push_back(guardian);
            this->setChildren(thirdSkill);
        };
};