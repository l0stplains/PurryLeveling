#pragma once
#include "skill/Skill.hpp"

class MageSkill : public Skill
{
protected:
    float intelligenceMultiplier;
    int   manaRegenBonus;

public:
    MageSkill(string                       name,
              float                        manaCost,
              int                          masteryCost,
              float                        damage,
              float                        effectChance,
              vector<unique_ptr<Effect>>&& effectVec,
              vector<unique_ptr<Skill>>&&  treeNodeVec,
              bool                         learn,
              bool                         activate,
              float                        intMul,
              int                          manaRegen)
        : Skill(name,
                manaCost,
                masteryCost,
                damage,
                effectChance,
                std::move(effectVec),
                std::move(treeNodeVec),
                learn,
                activate)
    {
        this->intelligenceMultiplier = intMul;
        this->manaRegenBonus         = manaRegen;
    };

    virtual float getIntelligenceMultiplier() const = 0;
    virtual int   getManaRegenBonus() const         = 0;
};