#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/Annihilation.hpp"
#include "effects/useEffects/BattleFocus.hpp"

class Devastation : public Skill
{
    private:
        // Same set of attributes as parent but with updated values
        float rageMultiplier = 0.25f;          // Major rage boost
        float lifeStealFactor = 0.0f;          // No life steal
        float armorPenetration = 0.15f;        // Adds armor penetration
        float selfDamage = 0.0f;               // No self damage

    public:
        Devastation() : Skill("Devastation", 9, 4, 12, 1.0f, {}, {}, false, false)
        {
            // Add BattleFocus effect
            vector<Effect> effectVec;
            effectVec.push_back(BattleFocus(1.25f, 0.15f, 0.10f, 2)); // 125% accuracy, +15% crit, -10% dodge, 2 turns
            this->setEffects(effectVec);
            
            // One child for an ultimate ability
            vector<Skill*> thirdSkill;
            Annihilation* annihilation = new Annihilation();
            thirdSkill.push_back(annihilation);
            this->setChildren(thirdSkill);
        }
};
