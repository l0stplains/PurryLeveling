#include "skill/SkillTree.hpp"

SkillTree::SkillTree(std::unique_ptr<Skill> rootNode) : skill(std::move(rootNode)) {}

std::vector<Skill*> SkillTree::getActiveSkill(const Skill* skill) const
{
    std::vector<Skill*> activeSkill;

    if (skill == nullptr)
    {
        if (this->skill != nullptr)
        {
            skill = this->skill.get();
        }
        else
        {
            return activeSkill;
        }
    }

    if (skill->getActivated())
    {
        activeSkill.push_back(const_cast<Skill*>(skill));
    }

    for (const auto& childPtr : skill->getChildren())
    {
        std::vector<Skill*> childActive = getActiveSkill(childPtr.get());
        activeSkill.insert(activeSkill.end(), childActive.begin(), childActive.end());
    }

    return activeSkill;
}

void SkillTree::setActive(std::string skillName, Skill* skill)
{
    if (skill == nullptr)
    {
        if (this->skill != nullptr)
        {
            skill = this->skill.get();
        }
        else
        {
            return;
        }
    }

    if (skillName == skill->getName())
    {
        skill->setActivated(true);
        skill->setIsLearned(true);
        return;
    }

    for (const auto& child : skill->getChildren())
    {
        setActive(skillName, child.get());
    }
}
