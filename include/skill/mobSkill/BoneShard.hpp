#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/Brittle.hpp"

// Skill Skeleton
class BoneShard : public Skill
{
    private:
        float damageMultiplier = 0.10f;
    
    public:
        BoneShard() : Skill("Bone Shard", 0, 0, 0, 1.0f, {}, {}, false, false)
        {
            vector<Effect> effectVec;
            effectVec.push_back(Brittle(15 * damageMultiplier));
            this->setEffects(effectVec);
        };

        void setDamageMultiplier(float multiplier) { damageMultiplier = multiplier; }
        float getDamageMultiplier() const { return damageMultiplier; }
        
};