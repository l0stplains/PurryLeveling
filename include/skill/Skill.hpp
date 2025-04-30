#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "effects/Effect.hpp"
#include "effects/Stats.hpp"
using namespace std;

class Skill
{
private:
    string                     name;
    float                      manaCost;
    int                        masteryCost;
    float                      damage;
    float                      effectChance;
    vector<unique_ptr<Effect>> effects;
    vector<Skill*>             children;
    bool                       isLearned;
    bool                       activated;

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
    Skill(string                     name,
          float                      manaCost,
          int                        masteryCost,
          float                      damage,
          float                      effectChance,
          vector<unique_ptr<Effect>> effectVec,
          vector<Skill*>             treeNodeVec,
          bool                       learn,
          bool                       activate);

    // Delete copy constructor and copy assignment operator
    Skill(const Skill&)            = delete;
    Skill& operator=(const Skill&) = delete;

    /* @brief Destructor for Skill
     */
    ~Skill();

    // -- Getter --
    string                            getName() const { return name; }
    float                             getManaCost() const { return manaCost; }
    int                               getMasteryCost() const { return masteryCost; }
    float                             getDamage() const { return damage; }
    float                             getEffectChance() const { return effectChance; }
    const vector<unique_ptr<Effect>>& getEffects() const { return effects; }
    void setEffects(vector<unique_ptr<Effect>>&& effectVec) { effects = std::move(effectVec); }
    vector<Skill*> getChildren() const { return children; }
    void           setChildren(vector<Skill*> treeNodeVec) { children = treeNodeVec; }
    bool           getIsLearned() const { return isLearned; }
    bool           getActivated() const { return activated; }
    void           setActivated(bool state) { activated = state; }
    void           setIsLearned(bool state) { isLearned = state; }

    /* @brief Learn the skill by activating the child skilTreeNode and deactivate the parent Skill
     * and change the isLearned state
     */
    bool learn(int* masteryPoint);
};