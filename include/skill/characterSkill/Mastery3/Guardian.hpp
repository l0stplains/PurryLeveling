#pragma once
#include <memory>

#include "effects/useEffects/FighterMight.hpp"
#include "skill/Skill.hpp"

// Skill Mastery3 Fighter
class Guardian : public Skill
{
private:
    float strengthMultiplier = 0.20f;  // Moderate strength boost
public:
    Guardian(bool isLearned = false, bool isActive = false)
        : Skill("Guardian", 15, 6, 5, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FighterMight>(15, 0.50f, 0.25f, 8, 40, 35, 4));
        this->setEffects(std::move(effectVec));
    }

    // Getter
    float getStrengthMultiplier() const { return strengthMultiplier; }
};