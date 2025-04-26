#include "SkillTreeNode.hpp"
#include "SkillTree.hpp"
#include "Effect.hpp"

using namespace std;

void traverseActivedSkil(SkillTreeNode* t)
{
    if (t == nullptr) {
        return;
    }
    if(t->activated == true)
    {
        cout << "Skill Name: " << t->name << endl;
        cout << "Mana Cost: " << t->manaCost << endl;
        cout << "Mastery Cost: " << t->masteryCost << endl;
        cout << "Damage: " << t->damage << endl;
        cout << "Effect Chance: " << t->effectChance << endl;
        cout << "Is Learned: " << (t->isLearned ? "Yes" : "No") << endl;
        cout << "Activated: " << (t->activated ? "Yes" : "No") << endl;
    }
    else{
        for (SkillTreeNode* child : t->children) {
            traverseActivedSkil(child);
        }
    }
}

int main() {
    // Test
    // Example there is a character typed Necromancer who have skill can lifesteal
    // Lifesteal have a child(upgrade version) that can heal more and all of its summon
    Effect lifesteal("Lifesteal", "Heals the caster for 50 percent of the damage dealt", 5.0);
    Effect lifestealUpgrade("Lifesteal Upgrade", "Heals the caster and all summons for 100 percent of the damage dealt", 10.0);
    SkillTreeNode* lifestealUpgradeNode = new SkillTreeNode("Lifesteal Upgrade", 20.0, 10, 200.0, 0.7, {lifestealUpgrade}, {}, false, false);
    SkillTreeNode* lifestealNode = new SkillTreeNode("Lifesteal", 10.0, 5, 100.0, 0.5, {lifesteal}, {lifestealUpgradeNode}, true, true); // root atau base skill
    SkillTree skillTree(lifestealNode);

    // test sebelum upgrade skill mana yang aktif (traverse)
    cout << "Before Upgrade:" << endl;
    traverseActivedSkil(skillTree.skillTreeNode);

    // Upgrade skill
    skillTree.skillTreeNode->learn();
    cout << "After Upgrade:" << endl;
    traverseActivedSkil(skillTree.skillTreeNode);
}