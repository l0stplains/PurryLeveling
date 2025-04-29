#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Guardian.hpp"
#include "effects/useEffects/FighterMight.hpp"

// Fighter Master 2
class IronWill : public Skill
{
private:
    float strengthMultiplier = 0.15f;  // Modest strength boost

public:
    IronWill() : Skill("Iron Will", 7, 3, 0, 0.6f, {}, {}, false, false)
    {
        vector<Skill*> thirdSkill;
        Guardian* guardian = new Guardian();
        thirdSkill.push_back(guardian);
        this->setChildren(thirdSkill);

        // Add FighterMight effect
        vector<Effect> effectVec;
        effectVec.push_back(FighterMight(10, 0.05f, 0.01f, 4, 25, 15, 2));
        this->setEffects(effectVec);
    }
    
    float getStrengthMultiplier() const { return strengthMultiplier; }
};