#ifndef SKILLTREE_HPP
#define SKILLTREE_HPP

#include "skill/Skill.hpp"

class SkillTree
{
private:
    Skill* skill;

public:
    // -- 4 Sekawan
    SkillTree(Skill* rootNode);
    SkillTree(const SkillTree& other);
    ~SkillTree();

    // - Getter
    Skill* getSkill() const { return this->skill; }
};

#endif  // SKILLTREE_HPP