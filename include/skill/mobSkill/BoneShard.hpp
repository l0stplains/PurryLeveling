#pragma once
#include "effects/useEffects/Brittle.hpp"
#include "skill/Skill.hpp"

// Skill Skeleton
class BoneShard : public Skill
{
private:
    float damageMultiplier = 0.10f;

public:
    BoneShard(bool isLearned = false, bool isActivated = false)
        : Skill("Bone Shard", 0, 0, 0, 1.0f, {}, {}, isLearned, isActivated)
    {
        vector<std::unique_ptr<Effect>> effectVec;
        effectVec.push_back(std::make_unique<Brittle>(15 * damageMultiplier));
        this->setEffects(std::move(effectVec));
    };

    void  setDamageMultiplier(float multiplier) { damageMultiplier = multiplier; }
    float getDamageMultiplier() const { return damageMultiplier; }
};