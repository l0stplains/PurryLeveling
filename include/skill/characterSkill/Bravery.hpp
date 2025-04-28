#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/IronWill.hpp"
#include "skill/characterSkill/Mastery2/Warcry.hpp"

// Root Skill Untuk Fighter
class Bravery : public Skill
{
private:
    float strengthMultiplier = 0.12f;
    float damageBoost        = 0.0f;
    float damageReduction    = 0.0f;
    float critChanceBonus    = 0.0f;
    float reflectDamage      = 0.0f;

public:
    Bravery() : Skill("Bravery", 4, 0, 0, 0, {}, {}, true, true)
    {
        vector<Skill*> secondSkill;

        WarCry*   warCry   = new WarCry();
        IronWill* ironWill = new IronWill();
        secondSkill.push_back(warCry);
        secondSkill.push_back(ironWill);
        this->setChildren(secondSkill);
    };
};