#include "SkillTree.hpp"

SkillTree::SkillTree(SkillTreeNode* rootNode) {
    this->skillTreeNode = rootNode;
}

SkillTree::SkillTree(const SkillTree& other) {
    this->skillTreeNode = new SkillTreeNode(*other.skillTreeNode);
}

SkillTree::~SkillTree() {
    delete skillTreeNode;
}