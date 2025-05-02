#include "skill/Skill.hpp"

Skill::Skill(string                     name,
             float                      manaCost,
             int                        masteryCost,
             float                      damage,
             float                      effectChance,
             vector<unique_ptr<Effect>> effectVec,
             vector<unique_ptr<Skill>>  treeNodeVec,
             bool                       learn,
             bool                       activate)
{
    this->name         = name;
    this->manaCost     = manaCost;
    this->masteryCost  = masteryCost;
    this->damage       = damage;
    this->effectChance = effectChance;
    this->effects      = std::move(effectVec);
    this->children     = std::move(treeNodeVec);
    this->isLearned    = learn;
    this->activated    = activate;
}

Skill::~Skill() {}

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
