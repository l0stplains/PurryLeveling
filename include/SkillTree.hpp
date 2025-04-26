#ifndef SKILLTREE_HPP
#define SKILLTREE_HPP

#include "SkillTreeNode.hpp"

class SkillTree
{
    public:
        SkillTreeNode* skillTreeNode;
        SkillTree(SkillTreeNode* rootNode);
        SkillTree(const SkillTree& other);
        ~SkillTree();
};

#endif // SKILLTREE_HPP