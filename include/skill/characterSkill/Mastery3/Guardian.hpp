#pragma once
#include "skill/Skill.hpp"
#include "effects/useEffects/FighterMight.hpp"

// Skill Mastery3 Fighter
class Guardian : public Skill
{
private:
    float strengthMultiplier = 0.20f;  // Moderate strength boost
public:
    Guardian() : Skill("Guardian", 15, 6, 5, 1.0f, {}, {}, false, false) 
    {
        vector<Effect> effectVec;
        effectVec.push_back(FighterMight(15, 0.50f, 0.25f, 8, 40, 35, 4));
        this->setEffects(effectVec);
    }
    
    // Getter
    float getStrengthMultiplier() const { return strengthMultiplier; }
};