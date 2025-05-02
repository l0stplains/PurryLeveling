#pragma once
#include <memory>

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
    SoulHarvest(bool isLearned = false, bool isActive = false)
        : Skill("Soul Harvest", 12, 5, 0, 0, {}, {}, isLearned, isActive)
    {
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Shadowstep>(0.2f, 0.25f, 0.15f, 3));
        this->setEffects(std::move(effectVec));
    }

    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int   getSummonedUnit() const { return summonedUnit; }
};
