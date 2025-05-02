#ifndef SKILLTREE_HPP
#define SKILLTREE_HPP

#include "skill/Skill.hpp"

class SkillTree
{
private:
    std::unique_ptr<Skill> skill;

public:
    // -- 4 Sekawan
    SkillTree(std::unique_ptr<Skill> rootNode);
    ~SkillTree();

    // - Getter
    const std::unique_ptr<Skill>& getSkill() const { return this->skill; }
};

#endif  // SKILLTREE_HPP