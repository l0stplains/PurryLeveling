#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/Bloodlust.hpp"
#include "skill/characterSkill/Mastery2/Devastation.hpp"

// Root Skill untuk 
class Rage : public Skill
{
    private:
        // All possible attributes for this skill tree, with base values
        float rageMultiplier = 0.125f;         // Base rage modifier
        float lifeStealFactor = 0.0f;          // No life steal initially
        float armorPenetration = 0.0f;         // No armor penetration initially
        float executeThreshold = 0.0f;         // No execution threshold initially
        float selfDamage = 0.0f;               // No self damage initially

    public:
        Rage() : Skill("Rage", 5, 0, 0, 0, {}, {}, true, true)
        {
            // Create second layer skills
            vector<Skill*> secondSkill;
            
            Bloodlust* bloodlust = new Bloodlust();
            Devastation* devastation = new Devastation();
            
            secondSkill.push_back(bloodlust);
            secondSkill.push_back(devastation);
            this->setChildren(secondSkill);
        };
};