#pragma once
#include "skill/Skill.hpp"

class NecromancerSkill : public Skill
{
protected:
    float lifestealPercentage;
    int   summonedUnit;

public:
    NecromancerSkill(string                       name,
                     float                        manaCost,
                     int                          masteryCost,
                     float                        damage,
                     float                        effectChance,
                     vector<unique_ptr<Effect>>&& effectVec,
                     vector<unique_ptr<Skill>>&&  treeNodeVec,
                     bool                         learn,
                     bool                         activate,
                     float                        lifeStealPercent,
                     int                          summon)
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
        this->lifestealPercentage = lifeStealPercent;
        this->summonedUnit        = summon;
    };

    virtual float getLifestealPercentage() const = 0;
    virtual int   getSummonedUnit() const        = 0;
};