#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/ArchmageWisdom.hpp"


class ArcaneInsight : public Skill
{
    private:
        float intelligenceMultiplier = 0.20f;   // Major intelligence boost
        float spellPowerBoost = 0.15f;          // Increases spell damage
        float manaCostReduction = 0.0f;        // Reduces mana costs by 0%
        int manaRegenBonus = 0;
        float timeWarpChance = 0.0f;           // Chance to reset spell cooldown

    public:
        ArcaneInsight() : Skill("Arcane Insight", 9, 3, 6, 1.0f, {}, {}, false, false)
        {
            vector<Skill*> thirdSkill;

            ArchmageWisdom* archmageWisdom = new ArchmageWisdom();
            thirdSkill.push_back(archmageWisdom);
            this->setChildren(thirdSkill);
        }
};
