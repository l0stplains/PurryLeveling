#pragma once
#include "skill/Skill.hpp"

class SoulSiphon : public Skill
{
    private:
        float intMultiplier = 1.25f;
        float lifestealPercentage = 0.8f;
        float lifeStealMultiplier = 0.3f;

    public:
        SoulSiphon() : Skill("Soul Siphon", 0, 0, 0, 0.0f, {}, {}, false, false){};

        float getIntMultiplier() const { return intMultiplier; }
        float getLifestealPercentage() const { return lifestealPercentage; }
        float getLifeStealMultiplier() const { return lifeStealMultiplier; }
        void setIntMultiplier(float multiplier) { intMultiplier = multiplier; }
        void setLifestealPercentage(float percentage) { lifestealPercentage = percentage; }
        void setLifeStealMultiplier(float multiplier) { lifeStealMultiplier = multiplier; }
};