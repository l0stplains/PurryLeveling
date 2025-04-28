#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/ShadowArtistry.hpp"

// Root Skill untuk Assassin
class Stealth : public Skill
{
private:
    // Base attributes for all stealth skills
    float agilityMultiplier = 0.17f;  // Base agility boost
    float chanceNewTurn     = 0.0f;   // No initial extra turn chance

public:
    Stealth() : Skill("Stealth", 3, 1, 0, 0, {}, {}, true, true)
    {
        vector<Skill*> secondSkill;

        ShadowArtistry* shadowArtistry = new ShadowArtistry();

        secondSkill.push_back(shadowArtistry);
        this->setChildren(secondSkill);
    };
};