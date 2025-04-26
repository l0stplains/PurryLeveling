#pragma once
#include "skill/Skill.hpp"

class FighterSkill : public Skill
{
    public:
        // Cuma iniialize stats untuk multiplier
        FighterSkill() : Skill("Bravery", 4, 0, 0, 0, {}, {}, true, true){
            Stats s;
            s.strength = 2;
            this -> setSkillStats(s);
        };
};