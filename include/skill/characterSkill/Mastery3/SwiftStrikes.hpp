#pragma once
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/Bloodthirst.hpp"
#include "skill/Skill.hpp"

// Skill Mastery3 Assassin
class SwiftStrikes : public Skill
{
private:
    // Same attributes as parent skill with enhanced values
    float agilityMultiplier = 0.25f;  // Good agility boost

public:
    SwiftStrikes() : Skill("Swift Strikes", 12, 5, 8, 1.0f, {}, {}, false, false)
    {
        vector<Effect> effectVec;
        effectVec.push_back(GladiatorMight(40, 13, 0.091f, 4));  // 2 extra attacks at 70% damage
        effectVec.push_back(Bloodthirst(20, 0.4, 0.1, 2));      // 25% chance to heal for 15% of damage dealt
        this->setEffects(effectVec);
    }
    
    // Getter
    float getAgilityMultiplier() const { return agilityMultiplier; }
};
