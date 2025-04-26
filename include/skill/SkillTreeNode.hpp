#include <string>
#include <vector>
#include "Effect.hpp"
using namespace std;

class SkillTreeNode
{
    private:
        string name;
        float manaCost;
        int masteryCost;
        float damage;
        float effectChance;
        vector<Effect> effects;
        vector<SkillTreeNode*> children;

    public:
        SkillTreeNode(string name, float manaCost, int masteryCost, float damage, float effectChance, vector<Effect> effectVec, vector<SkillTreeNode*> treeNodeVec);
        ~SkillTreeNode();
        void learn();
};