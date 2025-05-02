#pragma once
#include "skill/characterSkill/AssassinSkill.hpp"
#include "skill/characterSkill/Mastery2/ShadowArtistry.hpp"

// Root Skill untuk Assassin
class Stealth : public AssassinSkill
{
public:
    Stealth(bool isLearned = true, bool isActivated = true)
        : AssassinSkill("Stealth", 3, 1, 0, 0, {}, {}, isLearned, isActivated, 0.17f)
    {
        vector<unique_ptr<Skill>> secondSkill;
        secondSkill.push_back(std::make_unique<ShadowArtistry>());
        this->setChildren(std::move(secondSkill));
    }

    float getAgilityMultiplier() const override { return agilityMultiplier; }
};
