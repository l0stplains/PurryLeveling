#pragma once
#include "skill/Skill.hpp"

class FighterSkill : public Skill
{
protected:
    float strengthMultiplier;

public:
    FighterSkill(string                     name,
                 float                      manaCost,
                 int                        masteryCost,
                 float                      damage,
                 float                      effectChance,
                 vector<unique_ptr<Effect>> effectVec,
                 vector<Skill*>             treeNodeVec,
                 bool                       learn,
                 bool                       activate,
                 float                      strMul)
        : Skill(name, manaCost, masteryCost, damage, effectChance, effectVec, treeNodeVec, learn, activate)
    {
        this->strengthMultiplier = strMul;
    };

    virtual float getStrengthMultiplier() const = 0;
};