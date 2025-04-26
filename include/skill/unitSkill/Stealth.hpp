#pragma once
#include "skill/Skill.hpp"
#include "skill/unitSkill/secondSkill/Stealth2.hpp"

class Stealth : public Skill
{
    private:
        // Base attributes for all stealth skills
        float agilityMultiplier = 0.17f;       // Base agility boost
        float chanceNewTurn = 0.0f;            // No initial extra turn chance

    public:
        Stealth() : Skill("Stealth", 3, 1, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> secondSkill;

            ShadowStep* shadowStep = new ShadowStep();
            Assassinate* assassinate = new Assassinate();
            
            secondSkill.push_back(shadowStep);
            secondSkill.push_back(assassinate);
            this->setChildren(secondSkill);
        };
};