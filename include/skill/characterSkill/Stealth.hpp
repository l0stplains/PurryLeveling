#pragma once
#include <memory>

#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/ShadowArtistry.hpp"

// Root Skill untuk Assassin
class Stealth : public Skill
{
private:
    // Base attributes for all stealth skills
    float agilityMultiplier = 0.17f;  // Base agility boost

public:
    Stealth(bool isLearned = true, bool isActivated = true)
        : Skill("Stealth", 3, 1, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> secondSkill;

        ShadowArtistry* shadowArtistry = new ShadowArtistry();

        secondSkill.push_back(shadowArtistry);
        this->setChildren(secondSkill);

        // No effects currently added, but would use the following pattern if needed:
        // this->addEffect(std::make_unique<EffectType>(parameters));
    };
    // Getters
    float getAgilityMultiplier() const { return agilityMultiplier; }
};