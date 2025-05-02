#pragma once

#include <memory>

#include "skill/characterSkill/AssassinSkill.hpp"
#include "skill/characterSkill/Mastery2/ShadowArtistry.hpp"

// Root Skill untuk Assassin
class Stealth : public AssassinSkill
{
public:
    Stealth(bool isLearned = true, bool isActivated = true)
        : AssassinSkill("Stealth", 3, 1, 0, 0, {}, {}, isLearned, isActivated, 0.17f)
    {
        vector<Skill*> secondSkill;

        ShadowArtistry* shadowArtistry = new ShadowArtistry();

        secondSkill.push_back(shadowArtistry);
        this->setChildren(secondSkill);
    };

    float getAgilityMultiplier() const { return agilityMultiplier; }
};