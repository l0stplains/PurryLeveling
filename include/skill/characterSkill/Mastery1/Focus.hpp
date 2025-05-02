#pragma once
#include "skill/characterSkill/Mastery2/ArcaneInsight.hpp"
#include "skill/characterSkill/Mastery2/MentalClarity.hpp"
#include "skill/characterSkill/Mastery2/SpiritualConnection.hpp"

// Root Skill Untuk Mage
class Focus : public MageSkill
{
public:
    Focus(bool isLearned = true, bool isActivated = true)
        : MageSkill("Focus", 5, 3, 0, 0, {}, {}, isLearned, isActivated, 0.15f, 0)
    {
        vector<Skill*> secondSkill;

        ArcaneInsight*       arcaneInsight       = new ArcaneInsight();
        SpiritualConnection* spiritualConnection = new SpiritualConnection();
        MentalClarity*       mentalClarity       = new MentalClarity();

        secondSkill.push_back(arcaneInsight);
        secondSkill.push_back(spiritualConnection);
        secondSkill.push_back(mentalClarity);
        this->setChildren(secondSkill);
    };

    float getIntelligenceMultiplier() const override { return intelligenceMultiplier; }
    int getManaRegenBonus() const override { return manaRegenBonus; }
};