#ifndef SKILLTREE_HPP
#define SKILLTREE_HPP

#include <memory>
#include <vector>

#include "skill/Skill.hpp"

class SkillTree
{
private:
    std::unique_ptr<Skill> skill;

public:
    SkillTree(std::unique_ptr<Skill> rootNode);
    SkillTree(const SkillTree&)            = delete;
    SkillTree& operator=(const SkillTree&) = delete;
    SkillTree(SkillTree&&)                 = default;
    SkillTree& operator=(SkillTree&&)      = default;
    ~SkillTree()                           = default;

    const std::unique_ptr<Skill>& getSkill() const { return this->skill; }
    std::vector<Skill*>           getActiveSkill(const Skill* skill = nullptr) const;
    std::vector<std::string>      getSkillName(const Skill* skill = nullptr) const;
    void                          setActive(std::string skillName, Skill* skill = nullptr);
};

#endif