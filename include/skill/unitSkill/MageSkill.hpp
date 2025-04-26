#pragma once
#include "skill/Skill.hpp"

class MageSkill : public Skill
{
    public:
        // Cuma iniialize stats untuk multiplier
        MageSkill() : Skill("Focus", 5, 3, 0, 0, {}, {}, true, true){
            Stats s;
            s.intelligence = 2;
            this -> setSkillStats(s);
        };
};