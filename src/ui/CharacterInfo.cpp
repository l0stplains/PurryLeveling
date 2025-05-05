#include "ui/CharacterInfo.hpp"

#include "units/characters/Assassin.hpp"
#include "units/characters/Berseker.hpp"
#include "units/characters/Fighter.hpp"
#include "units/characters/Mage.hpp"
#include "units/characters/Necromancer.hpp"

CharacterInfo::CharacterInfo(GameContext& gameContext, float windowWidth, float windowHeight)
    : m_windowWidth(windowWidth),
      m_windowHeight(windowHeight),
      m_gameContext(gameContext),
      m_titleColor(0.95f, 0.9f, 0.6f, 1.0f),            // Gold-ish for title
      m_statsColor(0.8f, 0.8f, 0.9f, 1.0f),             // Light blue-ish for stat names
      m_valueColor(1.0f, 1.0f, 1.0f, 1.0f),             // White for values
      m_backgroundColorDark(0.1f, 0.1f, 0.15f, 1.0f),   // Dark blue-ish background
      m_backgroundColorLight(0.2f, 0.2f, 0.25f, 1.0f),  // Slightly lighter blue for contrast
      m_borderColor(0.5f, 0.5f, 0.7f, 1.0f)             // Purple-ish border
{
    // Nothing to initialize
}

std::string CharacterInfo::getCharacterType(const Character& character) const
{
    // Check character type by dynamic casting
    if (dynamic_cast<const Assassin*>(&character) != nullptr)
        return "Assassin";
    else if (dynamic_cast<const Berseker*>(&character) != nullptr)
        return "Berserker";
    else if (dynamic_cast<const Fighter*>(&character) != nullptr)
        return "Fighter";
    else if (dynamic_cast<const Mage*>(&character) != nullptr)
        return "Mage";
    else if (dynamic_cast<const Necromancer*>(&character) != nullptr)
        return "Necromancer";
    else
        return "Unknown";
}

void CharacterInfo::render(const Character& character, const AnimatedUnit& animatedUnit)
{
    // Set window position and size in the upper left corner
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiCond_Always);

    // Window flags - add NoTitleBar to remove header
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoTitleBar;  // Added NoTitleBar flag

    // Style customization - semi-transparent background
    ImVec4 bgColor = m_backgroundColorDark;
    bgColor.w      = 0.8f;  // Reduce opacity to 80%

    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Border, m_borderColor);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);           // Round corners
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));  // Consistent padding

    // Begin character info window
    if (ImGui::Begin("Character Info", nullptr, flags))
    {
        // Character name and type
        std::string characterType = getCharacterType(character);
        std::string headerText    = animatedUnit.GetName() + " - " + characterType;

        ImGui::PushStyleColor(ImGuiCol_Text, m_titleColor);
        ImGui::TextWrapped("%s", headerText.c_str());
        ImGui::PopStyleColor();  // Text color

        ImGui::Separator();

        // Fixed alignment table layout for stats
        float tableWidth = ImGui::GetContentRegionAvail().x;
        float colWidth1  = tableWidth * 0.45f;  // Width for labels
        float colWidth2  = tableWidth * 0.55f;  // Width for values

        ImGui::BeginTable("StatsTable", 2);

        // Set column widths
        ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, colWidth1);
        ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthFixed, colWidth2);

        // Level Row
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushStyleColor(ImGuiCol_Text, m_statsColor);
        ImGui::Text("Level:");
        ImGui::TableSetColumnIndex(1);
        ImGui::PushStyleColor(ImGuiCol_Text, m_valueColor);
        ImGui::Text("%d", character.GetLevel());
        ImGui::PopStyleColor(2);  // Pop both text colors

        // Experience Row
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushStyleColor(ImGuiCol_Text, m_statsColor);
        ImGui::Text("Experience:");
        ImGui::TableSetColumnIndex(1);
        ImGui::PushStyleColor(ImGuiCol_Text, m_valueColor);
        ImGui::Text("%d", character.GetExp());
        ImGui::PopStyleColor(2);

        // Gold Row
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushStyleColor(ImGuiCol_Text, m_statsColor);
        ImGui::Text("Gold:");
        ImGui::TableSetColumnIndex(1);
        ImGui::PushStyleColor(ImGuiCol_Text, m_valueColor);
        ImGui::Text("%d", character.GetGold());
        ImGui::PopStyleColor(2);

        ImGui::EndTable();

        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();

        // Add more rounded corners for progress bars
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);  // Increase rounding for frames

        // Health bar with text inside
        float healthRatio = static_cast<float>(animatedUnit.GetHealth()) /
                            static_cast<float>(animatedUnit.GetMaxHealth());

        ImGui::PushStyleColor(ImGuiCol_Text, m_statsColor);
        ImGui::Text("HP:");
        ImGui::SameLine();
        ImGui::PopStyleColor();

        // Use red color for health bar with lower opacity
        ImVec4 healthBarColor   = ImVec4(0.9f, 0.2f, 0.2f, 0.9f);  // Slightly transparent red
        ImVec4 healthBarBgColor = ImVec4(0.2f, 0.1f, 0.1f, 0.6f);  // Darker red for background

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, healthBarColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, healthBarBgColor);  // Add background color

        // Create HP string to overlay on bar
        char hpOverlay[32];
        snprintf(hpOverlay,
                 sizeof(hpOverlay),
                 "%d/%d",
                 animatedUnit.GetHealth(),
                 animatedUnit.GetMaxHealth());

        // Reduced height progress bar with text inside
        ImGui::ProgressBar(healthRatio, ImVec2(-1, 20), "");  // Slightly taller for better rounding

        // Calculate position for text overlay
        ImVec2 barMin   = ImGui::GetItemRectMin();
        ImVec2 barMax   = ImGui::GetItemRectMax();
        ImVec2 textSize = ImGui::CalcTextSize(hpOverlay);
        ImVec2 textPos  = ImVec2(barMin.x + (barMax.x - barMin.x - textSize.x) * 0.5f,
                                barMin.y + (barMax.y - barMin.y - textSize.y) * 0.5f);

        // Draw text overlay on progress bar
        ImGui::GetWindowDrawList()->AddText(textPos,
                                            IM_COL32(255, 255, 255, 255),  // White text
                                            hpOverlay);

        ImGui::PopStyleColor(2);  // Pop health bar colors

        // Add small spacing between bars
        ImGui::Spacing();

        // Mana bar with text inside
        float manaRatio = static_cast<float>(animatedUnit.GetCurrentMana()) /
                          static_cast<float>(animatedUnit.GetMaxMana());

        ImGui::PushStyleColor(ImGuiCol_Text, m_statsColor);
        ImGui::Text("MP:");
        ImGui::SameLine();
        ImGui::PopStyleColor();

        // Use blue color for mana bar with lower opacity
        ImVec4 manaBarColor   = ImVec4(0.2f, 0.4f, 0.8f, 0.9f);   // Slightly transparent blue
        ImVec4 manaBarBgColor = ImVec4(0.1f, 0.15f, 0.3f, 0.6f);  // Darker blue for background

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, manaBarColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, manaBarBgColor);  // Add background color

        // Create MP string to overlay on bar
        char mpOverlay[32];
        snprintf(mpOverlay,
                 sizeof(mpOverlay),
                 "%d/%d",
                 animatedUnit.GetCurrentMana(),
                 animatedUnit.GetMaxMana());

        // Reduced height progress bar with text inside
        ImGui::ProgressBar(manaRatio, ImVec2(-1, 20), "");  // Slightly taller for better rounding

        // Calculate position for text overlay
        barMin   = ImGui::GetItemRectMin();
        barMax   = ImGui::GetItemRectMax();
        textSize = ImGui::CalcTextSize(mpOverlay);
        textPos  = ImVec2(barMin.x + (barMax.x - barMin.x - textSize.x) * 0.5f,
                         barMin.y + (barMax.y - barMin.y - textSize.y) * 0.5f);

        // Draw text overlay on progress bar
        ImGui::GetWindowDrawList()->AddText(textPos,
                                            IM_COL32(255, 255, 255, 255),  // White text
                                            mpOverlay);

        ImGui::PopStyleColor(2);  // Pop mana bar colors
        ImGui::PopStyleVar();     // Pop frame rounding
    }
    ImGui::End();

    // Pop all style colors and vars that were pushed
    ImGui::PopStyleVar(2);    // Pop window rounding and padding
    ImGui::PopStyleColor(2);  // Pop window styling colors
}