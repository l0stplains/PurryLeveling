#include "save/PlayerConfigSaver.hpp"
#include <filesystem>
#include <fstream>

bool PlayerConfigSaver::SaveToFolder(
    const std::string&   rootDir,
    const Character&     character,
    const Equipment&     equipment,
    const Backpack&      backpack,
    std::string&         lastError
) {
    namespace fs = std::filesystem;
    // 1) create output directory: rootDir/CHAR_NAME
    fs::path outDir = fs::path(rootDir) / character.GetName();
    if (!fs::exists(outDir) && !fs::create_directories(outDir)) {
        lastError = "Cannot create directory: " + outDir.string();
        return false;
    }

    // 2) equipment.txt: one “<SLOT> <ITEMID>” per line
    {
        std::ofstream eq(outDir / "equipment.txt");
        if (!eq) { lastError = "Failed to open equipment.txt"; return false; }
        eq << "WEAPON       " << equipment.getWeapon().getItemID()  << "\n";
        eq << "ARMOR_HEAD   " << equipment.getHeadArmor().getItemID() << "\n";
        eq << "ARMOR_BODY   " << equipment.getBodyArmor().getItemID() << "\n";
        eq << "ARMOR_FOOT   " << equipment.getFootArmor().getItemID() << "\n";
        eq << "PENDANT      " << equipment.getPendant().getItemID()   << "\n";
    }

    // 3) backpack.txt: “<ROW> <COL> <ITEMID> <COUNT>” per non-empty cell
    {
        std::ofstream bp(outDir / "backpack.txt");
        if (!bp) { lastError = "Failed to open backpack.txt"; return false; }
        const int rows = 8, cols = 4;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int cnt = backpack.getQuantityAtTile(r, c);
                if (cnt <= 0) continue;
                auto itm = backpack.getItemAtTile(r, c);
                bp << r << " "
                   << c << " "
                   << itm.getItemID() << " "
                   << cnt << "\n";
            }
        }
    }

    return true;
}
