#pragma once
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/Skill.hpp"

// Skill Mastery3 Necromancer
class Arise : public Skill
{
private:
    float intelligenceMultiplier = 0.14f;
    int   summonedUnit           = 3;

public:
    // Cuma iniialize stats untuk multiplier
    Arise(bool isLearned = false, bool isActive = false) 
        : Skill("Arise", 13, 6, 0, 0, {}, {}, isLearned, isActive)
    {
        vector<Effect> effectVec;
        effectVec.push_back(TimeWarp(0.25f, 0.15f, 3));
        this->setEffects(effectVec);
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getSummonedUnit() const { return summonedUnit; }
};
