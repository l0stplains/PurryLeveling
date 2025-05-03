#pragma once
#include "skill/Skill.hpp"

class FighterSkill : public Skill
{
protected:
    float strengthMultiplier;

public:
    FighterSkill(string                       name,
                 float                        manaCost,
                 int                          masteryCost,
                 float                        damage,
                 float                        effectChance,
                 vector<unique_ptr<Effect>>&& effectVec,
                 vector<unique_ptr<Skill>>&&  treeNodeVec,
                 bool                         learn,
                 bool                         activate,
                 float                        strMul)
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
        this->strengthMultiplier = strMul;
    };

    virtual float getStrengthMultiplier() const = 0;
};