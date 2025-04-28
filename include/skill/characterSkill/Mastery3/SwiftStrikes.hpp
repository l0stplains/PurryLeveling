#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/TimeWarp.hpp"

class SwiftStrikes : public Skill
{
    private:
        // Same attributes as parent skill with enhanced values
        float agilityMultiplier = 0.25f;       // Good agility boost
        float chanceNewTurn = 0.10f;           // 10% chance for an extra turn

    public:
        SwiftStrikes() : Skill("Swift Strikes", 12, 5, 8, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(GladiatorMight(40, 13, 0.091f,4)); // 2 extra attacks at 70% damage for 3 turns
            this->setEffects(effectVec);
        }
};
