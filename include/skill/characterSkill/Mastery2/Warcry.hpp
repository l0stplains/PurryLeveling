#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Howl.hpp"

class WarCry : public Skill
{
private:
    float strengthMultiplier = 0.20f;  // Enhanced strength boost
    float damageBoost        = 0.15f;  // Additional damage boost
    float damageReduction    = 0.0f;
    float critChanceBonus    = 0.0f;  // Increased critical hit chance
    float reflectDamage      = 0.0f;

public:
    WarCry() : Skill("War Cry", 8, 2, 5, 1.0f, {}, {}, false, false)
    {
        vector<Skill*> thirdSkill;

        Howl* howl = new Howl();
        thirdSkill.push_back(howl);
        this->setChildren(thirdSkill);
    };
};