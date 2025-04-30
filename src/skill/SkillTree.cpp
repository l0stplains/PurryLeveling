#include "skill/SkillTree.hpp"

SkillTree::SkillTree(Skill* rootNode)
{
    this->skill = rootNode;
}

SkillTree::~SkillTree()
{
    delete skill;
}