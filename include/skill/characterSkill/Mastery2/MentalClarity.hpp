#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/ManaMastery.hpp"

class MentalClarity : public Skill
{
private:
    float intelligenceMultiplier = 0.15f;  // Enhanced intelligence
    float manaCostReduction      = 0.20f;  // Reduces mana costs by 20%
    int   manaRegenBonus         = 0;
    float spellPowerBoost        = 0.0f;  // nambah damage intinya
    float timeWarpChance         = 0.0f;

public:
    MentalClarity() : Skill("Mental Clarity", 7, 2, 0, 1.0f, {}, {}, false, false)
    {
        vector<Skill*> thirdSkill;

        ManaMastery* manaMastery = new ManaMastery();
        thirdSkill.push_back(manaMastery);
        this->setChildren(thirdSkill);
    }
};
