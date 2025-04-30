#include "skill/Skill.hpp"

Skill::Skill(string         name,
             float          manaCost,
             int            masteryCost,
             float          damage,
             float          effectChance,
             vector<unique_ptr<Effect>> effectVec,
             vector<Skill*> treeNodeVec,
             bool           learn,
             bool           activate)
{
    this->name         = name;
    this->manaCost     = manaCost;
    this->masteryCost  = masteryCost;
    this->damage       = damage;
    this->effectChance = effectChance;
    this->effects      = std::move(effectVec);
    this->children     = treeNodeVec;
    this->isLearned    = learn;
    this->activated    = activate;
}

Skill::~Skill()
{
    for (Skill* child : children)
    {
        delete child;
    }
}

bool Skill::learn(int* masteryPoint)
{
    // If there are no children, nothing to learn
    if (children.empty()) {
        return false;
    }

    bool allChildrenLearned = true;
    bool anySkillLearned = false;

    for (Skill* child : children)
    {
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
                // Not enough mastery points for this skill
                // Continue checking other skills
            }
        }
        else
        {
            // Try to learn child's children
            if (child->learn(masteryPoint)) {
                anySkillLearned = true;
            }
        }
    }

    if (allChildrenLearned && !children.empty())
    {
        this->activated = false;
    }
    
    return anySkillLearned;
}