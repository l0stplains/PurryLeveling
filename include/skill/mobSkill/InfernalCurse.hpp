#pragma once
#include "effects/useEffects/CurseOfFragilityEffect.hpp"
#include "skill/Skill.hpp"

class InfernalCurse : public Skill
{
public:
    InfernalCurse(bool isLearned = false, bool isActivated = false) 
        : Skill("Infernal Curse", 0, 0, 0, 0.0f, {}, {}, isLearned, isActivated)
    {
        vector<Effect> effectVec;
        effectVec.push_back(CurseOfFragilityEffect(30, 0.45f, 5));  // Example values
        this->setEffects(effectVec);
    };
};