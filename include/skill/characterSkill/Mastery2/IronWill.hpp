#pragma once
#include <memory>

#include "effects/useEffects/FighterMight.hpp"
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Guardian.hpp"

// Fighter Master 2
class IronWill : public Skill
{
private:
    float strengthMultiplier = 0.15f;  // Modest strength boost

public:
    IronWill(bool isLearned = false, bool isActivated = false)
        : Skill("Iron Will", 7, 3, 0, 0.6f, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;
        Guardian*      guardian = new Guardian();
        thirdSkill.push_back(guardian);
        this->setChildren(thirdSkill);

        // Add FighterMight effect
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<FighterMight>(10, 0.05f, 0.01f, 4, 25, 15, 2));
        this->setEffects(std::move(effectVec));
    }

    float getStrengthMultiplier() const { return strengthMultiplier; }
};