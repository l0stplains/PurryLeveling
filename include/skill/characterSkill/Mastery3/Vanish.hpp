#pragma once
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/Bloodthirst.hpp"
#include "skill/Skill.hpp"
#include <memory>

// Skill Mastery3 Assassin
class Vanish : public Skill
{
private:
    float agilityMultiplier = 0.30f;

public:
    Vanish(bool isLearned = false, bool isActive = false) 
        : Skill("Vanish", 10, 5, 0, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Shadowstep>(78, 0.45f, 0.38f, 4));
        effectVec.push_back(make_unique<Bloodthirst>(5, 0.55, 0.2, 2));
        this->setEffects(std::move(effectVec));
    }
    
    // Getter
    float getAgilityMultiplier() const { return agilityMultiplier; }
};
