#include "save/PlayerConfigSaver.hpp"

#include <filesystem>
#include <fstream>

#include "exception/Exception.hpp"

bool PlayerConfigSaver::SaveToFolder(const std::string& rootDir,
                                     const Character&   character,
                                     const Equipment&   equipment,
                                     const Backpack&    backpack,
                                     std::string&       lastError)
{
    namespace fs = std::filesystem;
    // 1) create output directory: rootDir/CHAR_NAME
    fs::path outDir = fs::path(rootDir) / character.GetName();
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
        const int rows = 8, cols = 4;
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
        st << "STRENGTH " << 0 << "\n";
        st << "INTELLIGENCE " << 0 << "\n";
        st << "AGILITY " << 0 << "\n";
        st << "BUFF_MULTIPLIER " << 0 << "\n";
        st << "CRITICAL_STRIKE_MULTIPLIER " << 0 << "\n";
        st << "CRITICAL_STRIKE_CHANCE " << 0 << "\n";
        st << "SKIPTURNCHANCE " << 0 << "\n";
        st << "LUCK " << 0 << "\n";
        st << "PHYSICAL_DEFENSE " << 0 << "\n";
        st << "MAGIC_DEFENSE " << 0 << "\n";
        st << "DODGE_CHANCE " << 0 << "\n";
        st << "ACCURACY " << 0 << "\n";
        st << "STATUS_RESISTANCE " << 0 << "\n";
        st << "HASTE_MULTIPLIER " << 0 << "\n";
        st << "RESOURCE_COST_MULTIPLIER " << 0 << "\n";

        // runtime class + extras
        std::string type = "Character";
        if (auto f = dynamic_cast<const Fighter*>(&character))
        {
            st << "TYPE " << "Fighter" << "\n";
            st << "BLOCK_CHANCE " << 0 << "\n";
        }
        else if (auto m = dynamic_cast<const Mage*>(&character))
        {
            st << "TYPE " << "Mage" << "\n";
            st << "EXTRA_MANA " << 0 << "\n";
        }
        else if (auto a = dynamic_cast<const Assassin*>(&character))
        {
            st << "TYPE " << "Assassin" << "\n";
            st << "CRITICAL_CHANCE " << 0 << "\n";
            st << "CRITICAL_MULTIPLIER " << 0 << "\n";
        }
        else if (auto b = dynamic_cast<const Berseker*>(&character))
        {
            st << "TYPE " << "Berseker" << "\n";
            st << "RAGE_MULTIPLIER " << 0 << "\n";
        }
        else if (auto n = dynamic_cast<const Necromancer*>(&character))
        {
            st << "TYPE " << "Necromancer" << "\n";
            st << "SUMMON " << 0 << "\n";
            st << "SUMMON_CHANCE " << 0 << "\n";
        }
    }

    return true;
}
