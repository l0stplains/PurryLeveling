#pragma once
#include "effects/useEffects/IronCurtain.hpp"
#include "skill/Skill.hpp"

class Howl : public Skill
{
private:
    float strengthMultiplier = 0.35f;  // Major strength boost
    float damageBoost        = 0.0f;
    float damageReduction    = 0.0f;
    float critChanceBonus    = 0.25f;  // Increased critical hit chance
    float reflectDamage      = 0.20f;

public:
    Howl() : Skill("Howl", 12, 5, 15, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        // Assuming Rage effect exists in your codebase or creating one
        effectVec.push_back(IronCurtain(30.0f, 25.0f, 0.20f, 5));
        this->setEffects(effectVec);
    }
};