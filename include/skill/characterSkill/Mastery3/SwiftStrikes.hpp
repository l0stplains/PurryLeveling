#pragma once
#include <memory>

#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "skill/Skill.hpp"

// Skill Mastery3 Assassin
class SwiftStrikes : public Skill
{
private:
    // Same attributes as parent skill with enhanced values
    float agilityMultiplier = 0.25f;  // Good agility boost

public:
    SwiftStrikes(bool isLearned = false, bool isActive = false)
        : Skill("Swift Strikes", 12, 5, 8, 1.0f, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<GladiatorMight>(40, 13, 0.091f, 4));  // 2 extra attacks at
                                                                              // 70% damage
        effectVec.push_back(make_unique<Bloodthirst>(20, 0.4, 0.1, 2));  // 25% chance to heal for
                                                                         // 15% of damage dealt
        this->setEffects(std::move(effectVec));
    }

    // Getter
    float getAgilityMultiplier() const { return agilityMultiplier; }
};
