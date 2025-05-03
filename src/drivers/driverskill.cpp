#include <iostream>

#include "effects/Effect.hpp"
#include "skill/SkillTree.hpp"
#include "skill/characterSkill/Mastery1/LifeSteal.hpp"

void printActiveSkills(const SkillTree& tree)
{
    auto active = tree.getActiveSkill();
    std::cout << "Active Skills:\n";
    for (const auto& s : active)
    {
        std::cout << " - " << s->getName() << "\n";
    }
}

int main()
{
    // Step 1: Construct the root skill (LifeSteal) which adds 2 children in its constructor
    std::unique_ptr<Skill> rootSkill = std::make_unique<LifeSteal>();

    // Step 2: Construct the skill tree with root
    SkillTree tree(std::move(rootSkill));

    std::cout << "=== Initial Tree State ===\n";
    printActiveSkills(tree);

    // Step 3: Manually deactivate root skill
    tree.getSkill()->setActivated(false);
    tree.getSkill()->setIsLearned(false);

    std::cout << "\n=== After Deactivating Root ===\n";
    printActiveSkills(tree);

    // Step 4: Attempt to activate a valid child skill
    std::cout << "\nActivating 'BloodRitual'...\n";
    tree.setActive("BloodRitual");
    printActiveSkills(tree);

    // Step 5: Attempt to activate an invalid skill
    std::cout << "\nActivating non-existent skill 'DarkFlame'...\n";
    tree.setActive("DarkFlame");  // Should do nothing
    printActiveSkills(tree);

    // Step 6: Show child names of the root
    std::cout << "\nRoot's Children:\n";
    const auto& children = tree.getSkill()->getChildren();
    for (const auto& child : children)
    {
        std::cout << " - " << child->getName() << "\n";
    }

    // Step 7: Add an Effect to BloodRitual
    std::vector<std::unique_ptr<Effect>> effects;
    effects.emplace_back(std::make_unique<Effect>("Burn", "Deals fire damage over time", 3));
    children[0]->setEffects(std::move(effects));  // Assuming BloodRitual is first

    std::cout << "\nEffects in 'BloodRitual':\n";
    for (const auto& effect : children[0]->getEffects())
    {
        std::cout << " - " << effect->GetName() << ": " << effect->GetDescription()
                  << " (Duration: " << effect->GetDuration() << " turns)\n";
    }

    // Step 8: Validate if learning affects mastery points (simulate learning)
    int  masteryPoint = 10;
    bool learned      = children[1]->learn(&masteryPoint);  // Try to learn 'UndeadLegion'
    std::cout << "\nLearning 'UndeadLegion' result: " << (learned ? "Success" : "Fail")
              << ", remaining mastery points: " << masteryPoint << "\n";

    return 0;
}
