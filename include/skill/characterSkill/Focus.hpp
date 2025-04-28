#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery2/ArcaneInsight.hpp"
#include "skill/characterSkill/Mastery2/MentalClarity.hpp"
#include "skill/characterSkill/Mastery2/SpiritualConnection.hpp"


// Root Skill Untuk Mage
class Focus : public Skill
{
    // Mengurangi manacost dan meningkatkan mana berdasarkan intelligence
    private:
        float intelligenceMultiplier = 0.15f;   // Massive intelligence boost
        float timeWarpChance = 0.0f;           // Chance to reset spell cooldown
        float spellPowerBoost = 0.0f;          // Increases spell damage
        float manaCostReduction = 0.0f;        // Reduces mana costs by 0%
        int manaRegenBonus = 0;

    public:
        // Cuma iniialize stats untuk multiplier
        Focus() : Skill("Focus", 5, 3, 0, 0, {}, {}, true, true)
        {
            vector<Skill*> secondSkill;

            ArcaneInsight* arcaneInsight = new ArcaneInsight();
            SpiritualConnection* spiritualConnection = new SpiritualConnection();
            MentalClarity* mentalClarity = new MentalClarity();

            secondSkill.push_back(arcaneInsight);
            secondSkill.push_back(spiritualConnection);
            secondSkill.push_back(mentalClarity);
            this->setChildren(secondSkill);
        };
};