#pragma once
#include "effects/useEffects/Shadowstep.hpp"
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/SwiftStrikes.hpp"

// Skill Mastery3 Assassin
class Vanish : public Skill
{
private:
    float agilityMultiplier = 0.30f;

public:
    Vanish() : Skill("Vanish", 10, 5, 0, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        effectVec.push_back(Shadowstep(78, 0.45f, 0.38f, 4));
        effectVec.push_back(Bloodthirst(5, 0.55, 0.2, 2));
        this->setEffects(effectVec);
    }
    
    // Getter
    float getAgilityMultiplier() const { return agilityMultiplier; }
};
