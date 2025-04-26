#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/TimeWarp.hpp"

class Vanish : public Skill
{
    private:
        // Same attributes as parent skill with enhanced values
        float agilityMultiplier = 0.30f;       // Major agility boost
        float chanceNewTurn = 0.20f;           // 20% chance for an extra turn

    public:
        Vanish() : Skill("Vanish", 10, 5, 0, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(Shadowstep(78, 0.45f, 0.38f, 4));
            this->setEffects(effectVec);
        };
};

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
        };
};

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
        };
};