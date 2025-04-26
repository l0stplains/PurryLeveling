#include "skill/Skill.hpp"

Skill::Skill(string name, float manaCost, int masteryCost, float damage, float effectChance, vector<Effect> effectVec, vector<Skill*> treeNodeVec, bool learn, bool activate) {
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

Skill::~Skill() {
    for (Skill* child : children) {
        delete child;
    }
}

void Skill::learn() {
    for (Skill* child : children) {
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

