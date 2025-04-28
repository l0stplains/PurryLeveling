#include "skill/Skill.hpp"
#include "skill/characterSkill/LifeSteal.hpp"
#include "skill/SkillTree.hpp"
#include "effects/Effect.hpp"

using namespace std;

void traverseActivedSkil(Skill* t)
{
    if (t == nullptr) {
        return;
    }
    if(t->getActivated()){ 
        cout << "Skill Name: " << t->getName() << endl;
        cout << "Mana Cost: " << t->getManaCost() << endl;
        cout << "Mastery Cost: " << t->getMasteryCost() << endl;
        cout << "Damage: " << t->getDamage() << endl;
        cout << "Effect Chance: " << t->getEffectChance() << endl;
        cout << "Is Learned: " << (t->getIsLearned() ? "Yes" : "No") << endl;
        cout << "Activated: " << (t->getActivated() ? "Yes" : "No") << endl;
    }
    
    else{
        for (Skill* child : t->getChildren()) {
            traverseActivedSkil(child);
        }
    }
}

int main() {
    int masteryPoint = 10;
    // Asumsi ini necromancer
    SkillTree* necromancerSkillTree = new SkillTree(new LifeSteal());
    Skill* rootSkill = necromancerSkillTree->getSkill();

    cout << "Intial Skill Active : " << endl;
    traverseActivedSkil(rootSkill);

    masteryPoint--;

}