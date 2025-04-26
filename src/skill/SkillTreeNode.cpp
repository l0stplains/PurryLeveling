#include "SkillTreeNode.hpp"

SkillTreeNode::SkillTreeNode(string name, float manaCost, int masteryCost, float damage, float effectChance, vector<Effect> effectVec, vector<SkillTreeNode*> treeNodeVec) {
    this->name = name;
    this->manaCost = manaCost;
    this->masteryCost = masteryCost;
    this->damage = damage;
    this->effectChance = effectChance;
    this->effects = effectVec;
    this->children = treeNodeVec;
}

SkillTreeNode::~SkillTreeNode() {
    for (SkillTreeNode* child : children) {
        delete child;
    }
}

void SkillTreeNode::learn() {
    // Debug
    cout << "Learning skill: " << name << endl;
    for (Effect& effect : effects) {
        effect.apply();
    }
}
