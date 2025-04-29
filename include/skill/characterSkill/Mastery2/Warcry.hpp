#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Howl.hpp"
#include "effects/useEffects/FighterMight.hpp"

// Fighter Mastery 2
class WarCry : public Skill
{
private:
    float strengthMultiplier = 0.20f;  // Enhanced strength boost

public:
    WarCry(bool isLearned = false, bool isActivated = false) 
        : Skill("War Cry", 8, 2, 5, 1.0f, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;
        Howl* howl = new Howl();
        thirdSkill.push_back(howl);
        this->setChildren(thirdSkill);

        // Add FighterMight effect
        vector<Effect> effectVec;
        effectVec.push_back(FighterMight(15, 0.5f, 0.0f, 5, 0, 0, 3));
        this->setEffects(effectVec);
    };
    
    float getStrengthMultiplier() const { return strengthMultiplier; }
};