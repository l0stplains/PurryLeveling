#include "effects/Effect.hpp"
#include "effects/Stats.hpp"
#include "skill/Skill.hpp"
#include "skill/SkillTree.hpp"
#include <random>
#include <iostream>
#include <vector>
// Anggap char kita necromancer
#include "skill/characterSkill/LifeSteal.hpp"

using namespace std;

void printDataSkill(Skill* skill)
{
    cout << "Skill Name: " << skill->getName() << endl;
    cout << "Mana Cost: " << skill->getManaCost() << endl;
    cout << "Mastery Cost: " << skill->getMasteryCost() << endl;
    cout << "Damage: " << skill->getDamage() << endl;
    cout << "Effect Chance: " << skill->getEffectChance() << endl;
    cout << "Is Learned: " << (skill->getIsLearned() ? "Yes" : "No") << endl;
    cout << "Is Activated: " << (skill->getActivated() ? "Yes" : "No") << endl;

    // Print effects
    for (const auto& effect : skill->getEffects())
    {
        cout << "- Effect Name: " << effect->GetName() << endl;
        cout << "- Effect Description: " << effect->GetDescription() << endl;
        cout << "- Effect Duration: " << effect->GetDuration() << endl;
    }
    cout << endl;
    for (const auto& child : skill->getChildren())
    {
        printDataSkill(child);
    }
}
 
// Print skill tree dalam bentuk tree
void printSkillTree(Skill* skill, int depth = 0)
{
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << skill->getName() << endl;
    for (const auto& child : skill->getChildren())
    {
        printSkillTree(child, depth + 1);
    }
}

// Print nama skill yang di learn dalam skilltree, traverse dari root ke semua child
void printLearnedSkills(Skill* skill, int depth = 0)
{
    if (skill->getIsLearned())
    {
        for (int i = 0; i < depth; ++i) cout << "  ";
        cout << skill->getName() << endl;
    }
    for (const auto& child : skill->getChildren())
    {
        printLearnedSkills(child, depth + 1);
    }
}

// Print skill yang aktif dalan bentuk list biasa
void printActiveSkills(Skill* skill)
{
    if (skill->getActivated())
    {
        cout << skill->getName() << endl;
    }
    for (const auto& child : skill->getChildren())
    {
        printActiveSkills(child);
    }
}   


// Func UseSkill
// bikin Activated skill trus randomize skill yang didapat, print effect nya
// Helper function to collect all active skills
vector<Skill*> collectActiveSkills(Skill* skill) {
    vector<Skill*> activeSkills;
    
    if (skill->getActivated()) {
        activeSkills.push_back(skill);
    }
    
    for (const auto& child : skill->getChildren()) {
        vector<Skill*> childActiveSkills = collectActiveSkills(child);
        activeSkills.insert(activeSkills.end(), childActiveSkills.begin(), childActiveSkills.end());
    }
    
    return activeSkills;
}

void useSkill(Skill* rootSkill) {
    // Collect all active skills
    vector<Skill*> activeSkills = collectActiveSkills(rootSkill);
    
    if (activeSkills.empty()) {
        cout << "No active skills available to use." << endl;
        return;
    }
    
    // List all active skills
    cout << "Available Active Skills:" << endl;
    for (size_t i = 0; i < activeSkills.size(); ++i) {
        cout << i + 1 << ". " << activeSkills[i]->getName() << endl;
    }
    
    // Randomly select a skill
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, activeSkills.size() - 1);
    int selectedIndex = dist(gen);
    
    Skill* selectedSkill = activeSkills[selectedIndex];
    
    // Print the selected skill and its effects
    cout << "\nUsing random skill: " << selectedSkill->getName() << endl;
    cout << "Damage: " << selectedSkill->getDamage() << endl;
    cout << "Effect chance: " << selectedSkill->getEffectChance() << endl;
    
    // Print effects of the selected skill
    const auto& effects = selectedSkill->getEffects();
    if (effects.empty()) {
        cout << "This skill has no additional effects." << endl;
    } else {
        cout << "Applied Effects:" << endl;
        for (const auto& effect : effects) {
            cout << "- " << effect->GetName() << ": " 
                 << effect->GetDescription() 
                 << " (Duration: " << effect->GetDuration() << " turns)" << endl;
        }
    }
    
    cout << endl;
}


int main()
{
    LifeSteal* lifeSteal = new LifeSteal();
    SkillTree skillTree(lifeSteal);

    // Test semua kemungkinan yang bisa kaya print semua effect yang ada di active skill sekarang dan testing learn funciton
    // Skill Tree
    cout << "Skill Tree:" << endl;
    printSkillTree(lifeSteal);
    cout << endl;
    // No upgrade
    cout << "Learned Skills:" << endl;
    printLearnedSkills(lifeSteal);
    cout << endl;

    // Upgrade pertama
    int masteryPoint = 20;
    lifeSteal->learn(&masteryPoint);
    cout << "Learned Skills after first upgrade:" << endl;
    printLearnedSkills(lifeSteal);
    cout << "Aktif :" << endl;
    printActiveSkills(lifeSteal);
    // print sisa mastery point
    cout << "Remaining Mastery Points: " << masteryPoint << endl;
    cout << endl;

    // Upgrade kedua
    lifeSteal->learn(&masteryPoint);
    cout << "Learned Skills after second upgrade:" << endl;
    printLearnedSkills(lifeSteal);

    // Testing Pake Effect yang tersedia 
    cout << "Test Use Skill:" << endl;
    useSkill(lifeSteal);
}