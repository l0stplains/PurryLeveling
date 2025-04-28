#include "SkillTree.hpp"

SkillTree::SkillTree(Skill* rootNode)
{
    this->skill = rootNode;
}

SkillTree::SkillTree(const SkillTree& other)
{
    this->skill = new Skill(*other.skill);
}

SkillTree::~SkillTree()
{
    delete skill;
}