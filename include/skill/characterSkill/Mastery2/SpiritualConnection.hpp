#pragma once
#include <memory>

#include "effects/useEffects/Chronoflux.hpp"
#include "skill/characterSkill/MageSkill.hpp"
#include "skill/characterSkill/Mastery3/EtherealBond.hpp"

// Mage Mastery 2
class SpiritualConnection : public MageSkill
{
public:
    SpiritualConnection(bool isLearned = false, bool isActivated = false)
        : MageSkill("Spiritual Connection", 8, 3, 0, 1.0f, {}, {}, isLearned, isActivated, 0.14f, 30)
    {
        vector<Skill*> thirdSkill;

        EtherealBond* etherealBond = new EtherealBond();
        thirdSkill.push_back(etherealBond);
        this->setChildren(thirdSkill);

        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<Chronoflux>(0, 0.3, 3));
        this->setEffects(std::move(effectVec));
    }
    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int getManaRegenBonus() const override { return manaRegenBonus; }
};
