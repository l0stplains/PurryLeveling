#include "SkillTreeNode.hpp"

SkillTreeNode::SkillTreeNode(string name, float manaCost, int masteryCost, float damage, float effectChance, vector<Effect> effectVec, vector<SkillTreeNode*> treeNodeVec, bool learn, bool activate) {
    this->name = name;
    this->manaCost = manaCost;
    this->masteryCost = masteryCost;
    this->damage = damage;
    this->effectChance = effectChance;
    this->effects = effectVec;
    this->children = treeNodeVec;
    this->isLearned = learn;
    this->activated = activate;
}

SkillTreeNode::~SkillTreeNode() {
    for (SkillTreeNode* child : children) {
        delete child;
    }
}

void SkillTreeNode::learn() {
    for (SkillTreeNode* child : children) {
        if(child->isLearned == false) {
            child->activated = true;
            child->isLearned = true;
            this-> activated = false;
        }
        else
        {
            child->learn();
        }
    }
}