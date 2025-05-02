#pragma once
#include "skill/Skill.hpp"

class BerserkerSkill : public Skill
{
protected:
    float furyMultiplier;
    float lifeStealFactor;
    float selfDamage;

public:
    BerserkerSkill(string                     name,
                   float                      manaCost,
                   int                        masteryCost,
                   float                      damage,
                   float                      effectChance,
                   vector<unique_ptr<Effect>> effectVec,
                   vector<unique_ptr<Skill>>  treeNodeVec,
                   bool                       learn,
                   bool                       activate,
                   float                      furMul,
                   float                      lifeSteal,
                   float                      selfDamage)
        : Skill(name, manaCost, masteryCost, damage, effectChance, effectVec, treeNodeVec, learn, activate)
    {
        this->furyMultiplier  = furMul;
        this->lifeStealFactor = lifeSteal;
        this->selfDamage      = selfDamage;
    }

    virtual float getFuryMultiplier() const  = 0;
    virtual float getLifeStealFactor() const = 0;
    virtual float getSelfDamage() const      = 0;
};