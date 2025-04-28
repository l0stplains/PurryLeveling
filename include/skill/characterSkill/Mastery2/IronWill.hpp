#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Guardian.hpp"

class IronWill : public Skill
{
    private:
        float strengthMultiplier = 0.15f;    // Modest strength boost
        float damageReduction = 0.20f;       // Damage reduction
        float damageBoost = 0.0f;  
        float critChanceBonus = 0.0f;       // Increased critical hit chance
        float reflectDamage = 0.0f; 

    public:
        IronWill() : Skill("Iron Will", 7, 3, 0, 1.0f, {}, {}, false, false)
        {
            vector<Skill*> thirdSkill;

            Guardian* guardian = new Guardian();
            thirdSkill.push_back(guardian);
            this->setChildren(thirdSkill);
        }
};