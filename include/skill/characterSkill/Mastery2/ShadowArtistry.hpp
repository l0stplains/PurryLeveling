#pragma once
#include <memory>

#include "effects/useEffects/BattleFocus.hpp"
#include "skill/characterSkill/AssassinSkill.hpp"
#include "skill/characterSkill/Mastery3/PhantomStrike.hpp"
#include "skill/characterSkill/Mastery3/SwiftStrikes.hpp"
#include "skill/characterSkill/Mastery3/Vanish.hpp"

// Assassin Mastery 2
class ShadowArtistry : public AssassinSkill
{
public:
    ShadowArtistry(bool isLearned = false, bool isActivated = false)
        : AssassinSkill("Shadow Artistry", 6, 3, 0, 0.5f, {}, {}, isLearned, isActivated, 0.17f)
    {
        vector<unique_ptr<Skill>> childSkills;
        childSkills.push_back(std::make_unique<Vanish>());
        childSkills.push_back(std::make_unique<PhantomStrike>());
        childSkills.push_back(std::make_unique<SwiftStrikes>());
        this->setChildren(std::move(childSkills));

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<BattleFocus>(0.25f, 0.35f, 0.1f, 3));
        this->setEffects(std::move(effectVec));
    }

    float getAgilityMultiplier() const override { return this->agilityMultiplier; }
};
