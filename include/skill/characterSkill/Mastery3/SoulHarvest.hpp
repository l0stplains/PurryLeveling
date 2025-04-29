#pragma once
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/TimeWarp.hpp"
#include "skill/Skill.hpp"

// Skill Mastery3 Necromancer
class SoulHarvest : public Skill
{
private:
    float intelligenceMultiplier = 0.25f;
    int   summonedUnit           = 1;

public:
    // Cuma iniialize stats untuk multiplier
    SoulHarvest(bool isLearned = true, bool isActive = true) 
        : Skill("Soul Harvest", 12, 5, 0, 0, {}, {}, isLearned, isActive)
    {
        vector<Effect> effectVec;
        effectVec.push_back(Shadowstep(20.0f, 0.25f, 0.15f, 3));
        this->setEffects(effectVec);
    }
    
    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getSummonedUnit() const { return summonedUnit; }
};
