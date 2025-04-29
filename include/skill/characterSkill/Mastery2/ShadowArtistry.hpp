#pragma once
#include "skill/Skill.hpp"
#include "skill/characterSkill/Mastery3/PhantomStrike.hpp"
#include "skill/characterSkill/Mastery3/SwiftStrikes.hpp"
#include "skill/characterSkill/Mastery3/Vanish.hpp"
#include "effects/useEffects/BattleFocus.hpp"

// Assassin Mastery 2
class ShadowArtistry : public Skill
{
private:
    // Maintain same attributes as Stealth
    float agilityMultiplier = 0.17f;

public:
    ShadowArtistry() : Skill("Shadow Artistry", 6, 3, 0, 0.5f, {}, {}, false, true)
    {
        vector<Skill*> childSkills;

        // Create child skills
        Vanish*        vanish        = new Vanish();
        PhantomStrike* phantomStrike = new PhantomStrike();
        SwiftStrikes*  swiftStrikes  = new SwiftStrikes();

        childSkills.push_back(vanish);
        childSkills.push_back(phantomStrike);
        childSkills.push_back(swiftStrikes);

        this->setChildren(childSkills);

        vector<Effect> effectVec;
        effectVec.push_back(BattleFocus(0.25f, 0.35f, 0.1f, 3)); 
        this->setEffects(effectVec);
    }
    
    float getAgilityMultiplier() const { return agilityMultiplier; }
};