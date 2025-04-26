#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Effect.hpp"
using namespace std;

class Skill
{
    private:
        string name;
        float manaCost;
        int masteryCost;
        float damage;
        float effectChance;
        vector<Effect> effects;
        vector<Skill*> children;
        bool isLearned;
        bool activated;

    public:
        /* @brief Constructor for Skill
         * @param name Name of the skill
         * @param manaCost Mana cost of the skill
         * @param masteryCost Mastery cost of the skill
         * @param damage Damage dealt by the skill
         * @param effectChance Chance of applying effects
         * @param effectVec Vector of effects associated with the skill
         * @param treeNodeVec Vector of child nodes in the skill tree
        */
       Skill(string name, float manaCost, int masteryCost, float damage, float effectChance, vector<Effect> effectVec, vector<Skill*> treeNodeVec, bool learn, bool activate);
        /* @brief Destructor for Skill
        */
        ~Skill();

        // -- Getter --
        string getName() const { return name; }
        float getManaCost() const { return manaCost; }
        int getMasteryCost() const { return masteryCost; }
        float getDamage() const { return damage; }
        float getEffectChance() const { return effectChance; }
        vector<Effect> getEffects() const { return effects; }
        vector<Skill*> getChildren() const { return children; }
        bool getIsLearned() const { return isLearned; }
        bool getActivated() const { return activated; }


        /* @brief Learn the skill by activating the child skilTreeNode and deactivate the parent Skill
         * and change the isLearned state
         */
        void learn();
};
