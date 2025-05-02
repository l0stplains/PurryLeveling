#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/EtherealBond.hpp"

// Mage Mastery 2
class SpiritualConnection : public Skill
{
private:
    float intelligenceMultiplier = 0.14f;  // Moderate intelligence boost
    int   manaRegenBonus         = 30;     // Mana regeneration

public:
    SpiritualConnection(bool isLearned = false, bool isActivated = false)
        : Skill("Spiritual Connection", 8, 3, 0, 1.0f, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> thirdSkill;

        EtherealBond* etherealBond = new EtherealBond();
        thirdSkill.push_back(etherealBond);
        this->setChildren(thirdSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Chronoflux>(0, 0.3, 3));
        this->setEffects(std::move(effectVec));
    }

    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int   getManaRegenBonus() const { return manaRegenBonus; }
};
