#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/ManaMastery.hpp"

// Mage Master 2
class MentalClarity : public Skill
{
private:
    float intelligenceMultiplier = 0.15f;  // Enhanced intelligence
    int   manaRegenBonus         = 0;

public:
    MentalClarity(bool isLearned = false, bool isActivated = false)
        : Skill("Mental Clarity", 7, 2, 0, 1.0f, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;

        ManaMastery* manaMastery = new ManaMastery();
        thirdSkill.push_back(manaMastery);
        this->setChildren(thirdSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Chronoflux>(0.2, 0, 3));
        this->setEffects(std::move(effectVec));
    }

    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int   getManaRegenBonus() const { return manaRegenBonus; }
};
