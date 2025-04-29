#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/IronWill.hpp"
#include "skill/characterSkill/Mastery2/Warcry.hpp"
#include <memory>

// Root Skill Untuk Fighter
class Bravery : public Skill
{
private:
    float strengthMultiplier = 0.12f;  // Untuk heal

public:
    Bravery(bool isLearned = true, bool isActivated = true) : Skill("Bravery", 4, 0, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> secondSkill;

        WarCry*   warCry   = new WarCry();
        IronWill* ironWill = new IronWill();
        secondSkill.push_back(warCry);
        secondSkill.push_back(ironWill);
        this->setChildren(secondSkill);
        
        // No effects currently added, but would use the following pattern if needed:
        // this->addEffect(std::make_unique<EffectType>(parameters));
    };

    float getStrengthMultiplier() const { return strengthMultiplier; }
};