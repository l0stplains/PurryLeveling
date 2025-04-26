#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/IronCurtain.hpp"


class Berserker : public Skill
{
    private:
        float strengthMultiplier = 0.35f;    // Major strength boost
        float damageBoost = 0.0f;           // Additional damage boost
        float damageReduction = 0.0f;
        float critChanceBonus = 0.25f;       // Increased critical hit chance
        float healthCost = 0.10f;            // Costs health to maintain
        float reflectDamage = 0.20f; 

    public:
        Berserker() : Skill("Berserker", 12, 5, 15, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            // Assuming Rage effect exists in your codebase or creating one
            effectVec.push_back(IronCurtain(30.0f, 25.0f, 0.20f, 5));
            this->setEffects(effectVec);
        };
};

class Guardian : public Skill
{
    private:
        float strengthMultiplier = 0.20f;     // Moderate strength boost
        float damageReduction = 0.35f;        // Major damage reduction
        float reflectDamage = 0.20f;          // Reflects portion of damage
        float critChanceBonus = 0.25f;       // Increased critical hit chance
        float healthCost = 0.10f;
        float damageBoost = 0.0f;

    public:
        Guardian() : Skill("Guardian", 15, 6, 5, 1.0f, {}, {}, false, false)
        {
        };
};