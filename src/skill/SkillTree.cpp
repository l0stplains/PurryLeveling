#include "skill/SkillTree.hpp"

SkillTree::SkillTree(std::unique_ptr<Skill> rootNode)
{
    this->skill = std::move(rootNode);
}

SkillTree::~SkillTree() {}