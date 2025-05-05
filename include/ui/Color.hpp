#pragma once

#include <imgui.h>

#include <string>

// Color utility class for consistent UI coloring across the application
class Color
{
public:
    // Get color for item based on item type
    static ImVec4 GetTypeColor(const std::string& itemType);

    // Get color for item rarity (S, A, B, C, D, E)
    static ImVec4 GetRarityColor(char rarity);
};