#ifndef SKILLTREENODE_HPP
#define SKILLTREENODE_HPP

#include <string>
#include <vector>
#include <iostream>
#include "Effect.hpp"
using namespace std;

class SkillTreeNode
{

    public:
        string name;
        float manaCost;
        int masteryCost;
        float damage;
        float effectChance;
        vector<Effect> effects;
        vector<SkillTreeNode*> children;
        bool isLearned;
        bool activated;
        /* @brief Constructor for SkillTreeNode
         * @param name Name of the skill
         * @param manaCost Mana cost of the skill
         * @param masteryCost Mastery cost of the skill
         * @param damage Damage dealt by the skill
         * @param effectChance Chance of applying effects
         * @param effectVec Vector of effects associated with the skill
         * @param treeNodeVec Vector of child nodes in the skill tree
        */
       SkillTreeNode(string name, float manaCost, int masteryCost, float damage, float effectChance, vector<Effect> effectVec, vector<SkillTreeNode*> treeNodeVec, bool learn, bool activate);
        /* @brief Destructor for SkillTreeNode
        */
        ~SkillTreeNode();
        /* @brief Learn the skill by activating the child skilTreeNode and deactivate the parent skillTreeNode
         * and change the isLearned state
         */
        void learn();
};

#endif // SKILLTREENODE_HPP