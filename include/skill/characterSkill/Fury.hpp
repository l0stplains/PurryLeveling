#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/Bloodlust.hpp"
#include "skill/characterSkill/Mastery2/Devastation.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include <memory>

// Root Skill untuk Berserker
class Fury : public Skill
{
    private:
        // All possible attributes for this skill tree, with base values
        float FuryMultiplier   = 0.125f;  // Base Fury modifier
        float lifeStealFactor  = 0.0f;    // No life steal initially
        float selfDamage       = 0.0f;    // No self damage initially

    public:
        Fury(bool isLearned = true, bool isActivated = true) : Skill("Fury", 5, 0, 0, 0, {}, {}, isLearned, isActivated)
        {
            // Create second layer skills
            vector<Skill*> secondSkill;

            Bloodlust*   bloodlust   = new Bloodlust();
            Devastation* devastation = new Devastation();

            secondSkill.push_back(bloodlust);
            secondSkill.push_back(devastation);
            this->setChildren(secondSkill);

            // Add effect using unique_ptr
            vector<unique_ptr<Effect>> effectVec;
            effectVec.push_back(make_unique<HarmoniousEmpowerment>(0, 0, FuryMultiplier, 3)); // Base effect
        };
        
        // Getters
        float getFuryMultiplier() const { return FuryMultiplier; }
        float getLifeStealFactor() const { return lifeStealFactor; }
        float getSelfDamage() const { return selfDamage; }
};