#include "SkillTreeNode.hpp"

class SkillTree
{
    private:
        SkillTreeNode* skillTreeNode;
    
    public:
        SkillTree(SkillTreeNode* rootNode);
        SkillTree(const SkillTree& other);
        ~SkillTree();
};