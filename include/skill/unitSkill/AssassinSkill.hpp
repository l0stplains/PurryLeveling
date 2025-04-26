#pragma once
#include "skill/Skill.hpp"

class AssassinSkill : public Skill
{
    public:
        // Cuma iniialize stats untuk multiplier
        AssassinSkill() : Skill("Stealth", 3, 0, 0, 0, {}, {}, true, true){
            Stats s;
            s.agility = 3;
            this -> setSkillStats(s);
        };
};