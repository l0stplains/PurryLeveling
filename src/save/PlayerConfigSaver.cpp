#include "save/PlayerConfigSaver.hpp"

#include <filesystem>
#include <fstream>
#include <functional>

#include "exception/Exception.hpp"
#include "skill/Skill.hpp"
#include "skill/SkillTree.hpp"

bool PlayerConfigSaver::SaveToFolder(const std::string& rootDir,
                                     const Character&   character,
                                     const Equipment&   equipment,
                                     const Backpack&    backpack,
                                     const Skill&       skillTree,
                                     std::string&       lastError)
{
    namespace fs = std::filesystem;
    // 1) create output directory: rootDir/CHAR_NAME
    fs::path outDir = fs::path(rootDir);
    if (!fs::exists(outDir) && !fs::create_directories(outDir))
    {
        lastError = "Cannot create directory: " + outDir.string();
        return false;
    }

    // 2) equipment.txt: one “<SLOT> <ITEMID>” per line
    {
        std::ofstream eq(outDir / "equipment.txt");
        if (!eq)
        {
            lastError = "Failed to open equipment.txt";
            return false;
        }
        eq << "WEAPON       " << equipment.getWeapon().getItemID() << "\n";
        eq << "ARMOR_HEAD   " << equipment.getHeadArmor().getItemID() << "\n";
        eq << "ARMOR_BODY   " << equipment.getBodyArmor().getItemID() << "\n";
        eq << "ARMOR_FOOT   " << equipment.getFootArmor().getItemID() << "\n";
        eq << "PENDANT      " << equipment.getPendant().getItemID() << "\n";
    }

    // 3) backpack.txt: “<ROW> <COL> <ITEMID> <COUNT>” per non-empty cell
    {
        std::ofstream bp(outDir / "backpack.txt");
        if (!bp)
        {
            lastError = "Failed to open backpack.txt";
            return false;
        }
        const int rows = 4, cols = 8;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                try
                {
                    int cnt = backpack.getQuantityAtTile(r, c);
                    if (cnt <= 0)
                        continue;
                    auto itm = backpack.getItemAtTile(r, c);
                    bp << r << " " << c << " " << itm.getItemID() << " " << cnt << "\n";
                }
                catch (const EmptyCellException&)
                {
                    // empty cell → skip
                    continue;
                }
            }
        }
    }

    // 4) stats.txt: LEVEL, EXP, GOLD, MASTERY, NAME, all Stats fields, TYPE, ATTACK_RANGE,
    // BLOCK_CHANCE
    {
        std::ofstream st(outDir / "stats.txt");
        if (!st)
        {
            lastError = "Failed to open stats.txt";
            return false;
        }

        // basic RPG fields
        st << "LEVEL " << character.GetLevel() << "\n";
        st << "EXP " << character.GetExp() << "\n";
        st << "GOLD " << character.GetGold() << "\n";
        st << "MASTERY " << character.GetMastery() << "\n";
        st << "NAME " << character.GetName() << "\n";

        // core Stats
        st << "STRENGTH " << character.GetStats().strength << "\n";
        st << "INTELLIGENCE " << character.GetStats().intelligence << "\n";
        st << "AGILITY " << character.GetStats().agility << "\n";
        st << "BUFF_MULTIPLIER " << character.GetStats().buffMultiplier << "\n";
        st << "CRITICAL_STRIKE_MULTIPLIER " << character.GetStats().criticalStrikeMultiplier << "\n";
        st << "CRITICAL_STRIKE_CHANCE " << character.GetStats().criticalStrikeChance << "\n";
        st << "SKIPTURNCHANCE " << character.GetStats().skipTurnChance << "\n";
        st << "LUCK " << character.GetStats().luck << "\n";
        st << "PHYSICAL_DEFENSE " << character.GetStats().physicalDefense << "\n";
        st << "MAGIC_DEFENSE " << character.GetStats().magicDefense << "\n";
        st << "DODGE_CHANCE " << character.GetStats().dodgeChance << "\n";
        st << "ACCURACY " << character.GetStats().accuracy << "\n";
        st << "STATUS_RESISTANCE " << character.GetStats().statusResistance << "\n";
        st << "HASTE_MULTIPLIER " << character.GetStats().hasteMultiplier << "\n";
        st << "RESOURCE_COST_MULTIPLIER " << character.GetStats().resourceCostMul << "\n";

        // runtime class + extras
        std::string type = "Character";
        if (auto f = dynamic_cast<const Fighter*>(&character))
        {
            st << "TYPE " << "Fighter" << "\n";
        }
        else if (auto m = dynamic_cast<const Mage*>(&character))
        {
            st << "TYPE " << "Mage" << "\n";
        }
        else if (auto a = dynamic_cast<const Assassin*>(&character))
        {
            st << "TYPE " << "Assassin" << "\n";
        }
        else if (auto b = dynamic_cast<const Berseker*>(&character))
        {
            st << "TYPE " << "Berseker" << "\n";
        }
        else if (auto n = dynamic_cast<const Necromancer*>(&character))
        {
            st << "TYPE " << "Necromancer" << "\n";
        }
    }

    // 4) skills.txt : column 0 are skills that are learned, column 1 are whether the skill is
    // activated
    {
        std::ofstream sk(outDir / "skills.txt");
        if (!sk)
        {
            lastError = "Failed to open skills.txt";
            return false;
        }

        // helper to walk the tree
        std::function<void(const Skill*)> dump = [&](const Skill* s) {
            sk << (s->getName()) << "\n";
            for (const auto& childPtr : s->getChildren())
                dump(childPtr.get());
        };

        // start at the root skill
        dump(&skillTree);
    }

    return true;
}
