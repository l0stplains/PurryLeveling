#pragma once
#include "skill/Skill.hpp"

class BerserkerSkill : public Skill
{
    public:
        // Cuma iniialize stats untuk multiplier
        BerserkerSkill() : Skill("Rage", 5, 0, 0, 0, {}, {}, true, true){
            Stats s;
            s.strength = 3;
            this -> setSkillStats(s);
        };
};