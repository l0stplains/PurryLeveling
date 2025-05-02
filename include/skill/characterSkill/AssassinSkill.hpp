#pragma once
#include "skill/Skill.hpp"

class AssassinSkill : public Skill
{
protected:
    float agilityMultiplier;

public:
    AssassinSkill(string                       name,
                  float                        manaCost,
                  int                          masteryCost,
                  float                        damage,
                  float                        effectChance,
                  vector<unique_ptr<Effect>>&& effectVec,
                  vector<unique_ptr<Skill>>&&  treeNodeVec,
                  bool                         learn,
                  bool                         activate,
                  float                        agilMul)
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
        this->agilityMultiplier = agilMul;
    };

    virtual float getAgilityMultiplier() const = 0;
};