#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/ArcaneInsight.hpp"
#include "skill/characterSkill/Mastery2/MentalClarity.hpp"
#include "skill/characterSkill/Mastery2/SpiritualConnection.hpp"
#include <memory>

// Root Skill Untuk Mage
class Focus : public Skill
{
    // Mengurangi manacost dan meningkatkan mana berdasarkan intelligence
private:
    float intelligenceMultiplier = 0.15f;  // Massive intelligence boost
    int   manaRegenBonus         = 0;

public:
    // Cuma iniialize stats untuk multiplier
    Focus(bool isLearned = true, bool isActivated = true) : Skill("Focus", 5, 3, 0, 0, {}, {}, isLearned, isActivated)
    {
        vector<Skill*> secondSkill;

        ArcaneInsight*       arcaneInsight       = new ArcaneInsight();
        SpiritualConnection* spiritualConnection = new SpiritualConnection();
        MentalClarity*       mentalClarity       = new MentalClarity();

        secondSkill.push_back(arcaneInsight);
        secondSkill.push_back(spiritualConnection);
        secondSkill.push_back(mentalClarity);
        this->setChildren(secondSkill);
        
        // No effects currently added, but would use the following pattern if needed:
        // this->addEffect(std::make_unique<EffectType>(parameters));
    };

    // Getters
    float getIntelligenceMultiplier() const { return intelligenceMultiplier; }
    int getManaRegenBonus() const { return manaRegenBonus; }
};