#pragma once
#include "effects/useEffects/IronCurtain.hpp"
#include "effects/useEffects/FighterMight.hpp"
#include "skill/Skill.hpp"
#include <memory>

// Skill Mastery3 Fighter
class Howl : public Skill
{
private:
    float strengthMultiplier = 0.35f;  // Major strength boost

public:
    Howl(bool isLearned = false, bool isActive = false) 
        : Skill("Howl", 12, 5, 15, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<IronCurtain>(30.0f, 25.0f, 0.20f, 5));
        effectVec.push_back(make_unique<FighterMight>(25, 0.5f, 0.25f, 6, 30, 20, 4));
        this->setEffects(std::move(effectVec));
    }
    
    // Getter
    float getStrengthMultiplier() const { return strengthMultiplier; }
};