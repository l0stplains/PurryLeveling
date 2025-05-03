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

    bool allChildrenLearned = true;
    bool anySkillLearned    = false;

    for (const auto& childPtr : children)
    {
        Skill* child = childPtr.get();

        if (!child->isLearned)
        {
            if (*masteryPoint >= child->getMasteryCost())
            {
                child->activated = true;
                child->isLearned = true;
                *masteryPoint -= child->getMasteryCost();
                anySkillLearned = true;
            }
            else
            {
                allChildrenLearned = false;
                // Continue checking other skills
            }
        }
        else
        {
            if (child->learn(masteryPoint))
            {
                anySkillLearned = true;
            }
        }
    }

    if (allChildrenLearned)
    {
        this->activated = false;
    }

    return anySkillLearned;
}
