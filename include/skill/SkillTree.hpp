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
    SkillTree(const SkillTree&)            = delete;
    SkillTree& operator=(const SkillTree&) = delete;
    SkillTree(SkillTree&&)                 = default;
    SkillTree& operator=(SkillTree&&)      = default;
    ~SkillTree()                           = default;  // let unique_ptr do the work

    // - Getter
    const std::unique_ptr<Skill>& getSkill() const { return this->skill; }
};

#endif  // SKILLTREE_HPP