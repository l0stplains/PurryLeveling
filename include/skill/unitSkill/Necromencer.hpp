#pragma once
#include "skill/Skill.hpp"

class NecromancerSkill : public Skill
{
    public:
        // Cuma iniialize stats untuk multiplier
        NecromancerSkill() : Skill("SoulAbsorb", 6, 0, 0, 0, {}, {}, true, true){
            Stats s;
            s.intelligence = 2;
            this -> setSkillStats(s);
        };
};