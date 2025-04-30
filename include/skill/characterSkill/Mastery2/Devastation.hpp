#pragma once
#include "effects/useEffects/BattleFocus.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Annihilation.hpp"
#include <memory>

// Berserker Master 2
class Devastation : public Skill
{
private:
    float rageMultiplier   = 0.25f;  // Major rage boost
    float lifeStealFactor  = 0.0f;   // No life steal
    float selfDamage       = 0.0f;   // No self damage

public:
    Devastation(bool isLearned = false, bool isActivated = false) 
        : Skill("Devastation", 9, 4, 12, 1.0f, {}, {}, isLearned, isActivated)
    {
        // Add effects using unique_ptr
        vector<unique_ptr<Effect>> effectVec;
        effectVec.push_back(make_unique<BattleFocus>(1.25f, 0.15f, 0.10f, 2));
        effectVec.push_back(make_unique<HarmoniousEmpowerment>(25, 10, rageMultiplier, 4));
        this->setEffects(std::move(effectVec));

        // One child for an ultimate ability
        vector<Skill*> thirdSkill;
        Annihilation*  annihilation = new Annihilation();
        thirdSkill.push_back(annihilation);
        this->setChildren(thirdSkill);
    }
    
    float getRageMultiplier() const { return rageMultiplier; }
    float getLifeStealFactor() const { return lifeStealFactor; }
    float getSelfDamage() const { return selfDamage; }
};
