#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/Bloodlust.hpp"
#include "skill/characterSkill/Mastery2/Devastation.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"

// Root Skill untuk Berserker
class Fury : public Skill
{
    private:
        // All possible attributes for this skill tree, with base values
        float FuryMultiplier   = 0.125f;  // Base Fury modifier
        float lifeStealFactor  = 0.0f;    // No life steal initially
        float selfDamage       = 0.0f;    // No self damage initially

    public:
        Fury() : Skill("Fury", 5, 0, 0, 0, {}, {}, true, true)
        {
            // Create second layer skills
            vector<Skill*> secondSkill;

            Bloodlust*   bloodlust   = new Bloodlust();
            Devastation* devastation = new Devastation();

            secondSkill.push_back(bloodlust);
            secondSkill.push_back(devastation);
            this->setChildren(secondSkill);

            vector<Effect> effectVec;
            effectVec.push_back(HarmoniousEmpowerment(5, 2, 0.2f, 3));
            this->setEffects(effectVec);
        };
        
        // Getters
        float getFuryMultiplier() const { return FuryMultiplier; }
        float getLifeStealFactor() const { return lifeStealFactor; }
        float getSelfDamage() const { return selfDamage; }
};