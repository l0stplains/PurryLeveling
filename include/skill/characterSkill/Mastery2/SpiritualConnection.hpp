#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/EtherealBond.hpp"

class SpiritualConnection : public Skill
{
    private:
        float intelligenceMultiplier = 0.14f;   // Moderate intelligence boost
        int manaRegenBonus = 30;            // Mana regeneration
        float manaCostReduction = 0.0f;        
        float spellPowerBoost = 0.0f;
        float timeWarpChance = 0.03f;

    public:
        SpiritualConnection() : Skill("Spiritual Connection", 8, 3, 0, 1.0f, {}, {}, false, false)
        {
            vector<Skill*> thirdSkill;

            EtherealBond* etherealBond = new EtherealBond();
            thirdSkill.push_back(etherealBond);
            this->setChildren(thirdSkill);
        }
};
