#include "skill/Skill.hpp"

Skill::Skill(string                       name,
             float                        manaCost,
             int                          masteryCost,
             float                        damage,
             float                        effectChance,
             vector<unique_ptr<Effect>>&& effectVec,
             vector<unique_ptr<Skill>>&&  treeNodeVec,
             bool                         learn,
             bool                         activate)
    : name(name),
      manaCost(manaCost),
      masteryCost(masteryCost),
      damage(damage),
      effectChance(effectChance),
      effects(std::move(effectVec)),
      children(std::move(treeNodeVec)),
      isLearned(learn),
      activated(activate)
{}

bool Skill::learn(int* masteryPoint)
{
    if (children.empty())
    {
        return false;
    }
    this -> isLearned = true;
    this -> activated = true;

    masteryPoint -= this->getMasteryCost();

    return true;
}
