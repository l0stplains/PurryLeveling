#pragma once
#include "skill/Skill.hpp"
#include "skill/unitSkill/thirdSkill/Focus3.hpp"

class MentalClarity : public Skill
{
    private:
        float intelligenceMultiplier = 0.15f;   // Enhanced intelligence
        float manaCostReduction = 0.20f;        // Reduces mana costs by 20%
        int manaRegenBonus = 0;
        float spellPowerBoost = 0.0f; // nambah damage intinya
        float timeWarpChance = 0.0f;

    public:
        MentalClarity() : Skill("Mental Clarity", 7, 2, 0, 1.0f, {}, {}, false, false)
        {
            vector<Skill*> thirdSkill;

            ManaMastery* manaMastery = new ManaMastery();
            thirdSkill.push_back(manaMastery);
            this->setChildren(thirdSkill);
        };
};

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
        };
};

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
        };
};