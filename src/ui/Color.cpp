#include "ui/Color.hpp"

ImVec4 Color::GetTypeColor(const std::string& itemType)
{
    // Equipment type colors
    if (itemType == "Potion")
        return ImVec4(0.2f, 0.8f, 0.2f, 1.0f);  // Green for potions
    else if (itemType == "Weapon")
        return ImVec4(0.8f, 0.2f, 0.2f, 1.0f);  // Red for weapons
    else if (itemType == "HeadArmor" || itemType == "BodyArmor" || itemType == "FootArmor")
        return ImVec4(0.2f, 0.6f, 0.8f, 1.0f);  // Blue for armor
    else if (itemType == "Pendant")
        return ImVec4(0.8f, 0.6f, 0.2f, 1.0f);  // Gold/yellow for pendants

    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // White for others
}

ImVec4 Color::GetRarityColor(char rarity)
{
    switch (rarity)
    {
        case 'S':
            return ImVec4(0.5f, 0.5f, 1.0f, 1.0f);  // Light blue for S
        case 'A':
            return ImVec4(0.5f, 1.0f, 0.5f, 1.0f);  // Light green for A
        case 'B':
            return ImVec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange for B
        case 'C':
            return ImVec4(1.0f, 0.5f, 0.5f, 1.0f);  // Light red for C
        case 'D':
            return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);  // Light grey for D
        case 'E':
            return ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // Darker grey for E
        default:
            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // White for others
    }
}