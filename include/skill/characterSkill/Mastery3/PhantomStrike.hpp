#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/TimeWarp.hpp"

class PhantomStrike : public Skill
{
    private:
        // Same attributes as parent skill with enhanced values
        float agilityMultiplier = 0.25f;       // Good agility boost
        float chanceNewTurn = 0.15f;           // 15% chance for an extra turn

    public:
        PhantomStrike() : Skill("Phantom Strike", 14, 6, 25, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(TimeWarp(0.40f, 0.131f, 3)); // 20% enemy skip turn, 50% damage boost, 2 turns
            this->setEffects(effectVec);
        }
};
