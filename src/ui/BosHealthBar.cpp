#include "ui/BossHealthBar.hpp"
BossHealthBar::BossHealthBar(float height) : m_height(height), m_isVisible(true)
{
    // Load a menacing font for the boss name if needed
    // m_font.loadFromFile("path/to/boss_font.ttf");
}

void BossHealthBar::show()
{
    m_isVisible = true;
}
void BossHealthBar::hide()
{
    m_isVisible = false;
}
bool BossHealthBar::isVisible() const
{
    return m_isVisible;
}

// Method to render the boss health bar at the top of the screen
void BossHealthBar::render(const Unit& boss)
{
    if (!m_isVisible)
        return;

    // Get viewport dimensions for positioning
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Calculate width based on percentage of screen width
    float barWidth = viewport->WorkSize.x * 0.5f;  // 50% of screen width

    // Position window centered at top of screen
    ImVec2 windowPos(viewport->WorkPos.x + (viewport->WorkSize.x - barWidth) * 0.5f,
                     viewport->WorkPos.y + 32.f);
    ImVec2 windowSize(barWidth, m_height);

    // Set window position and size
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    // Window flags: no decorations, no interactions
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
                             ImGuiWindowFlags_NoBackground;

    // Custom ImGui styling for the health bar
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.05f, 0.05f, 0.05f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.7f, 0.6f, 0.3f, 1.0f));         // Gold border
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));  // Red health
                                                                                    // bar

    // Begin the window
    if (ImGui::Begin("##BossHealthBar", nullptr, flags))
    {
        // Calculate health percentage
        float healthPercent = boss.GetMaxHealth() > 0 ? static_cast<float>(boss.GetHealth()) /
                                                            static_cast<float>(boss.GetMaxHealth())
                                                      : 0.0f;

        // Limit to valid range
        healthPercent = std::max(0.0f, std::min(healthPercent, 1.0f));

        // Display Boss Name with a larger, dramatic font
        // Create a scaled font for the boss name
        static ImFont* largeFont = nullptr;
        if (largeFont == nullptr && ImGui::GetIO().Fonts->Fonts.Size > 0)
        {
            largeFont = ImGui::GetIO().Fonts->Fonts[0];
        }

        // Push custom styling for the boss name
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));  // Gold text

        if (largeFont)
        {
            ImGui::PushFont(largeFont);
        }

        // Center the text
        auto nameSize = ImGui::CalcTextSize(boss.GetName().c_str());
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - nameSize.x) * 0.5f);

        ImGui::Text("%s", boss.GetName().c_str());

        // Pop font styling
        if (largeFont)
        {
            ImGui::PopFont();
        }
        ImGui::PopStyleColor();

        // Health display
        // Add padding for aesthetics
        float padX = ImGui::GetWindowWidth() * 0.05f;
        ImGui::SetCursorPosX(padX);
        ImGui::PushItemWidth(ImGui::GetWindowWidth() - padX * 2);

        // Health bar
        ImGui::ProgressBar(healthPercent, ImVec2(-1, 17), "");

        // Health text overlay centered on the bar
        std::string healthText =
            std::to_string(boss.GetHealth()) + " / " + std::to_string(boss.GetMaxHealth());
        auto healthTextSize = ImGui::CalcTextSize(healthText.c_str());

        float textPosX = padX + (ImGui::GetWindowWidth() - padX * 2 - healthTextSize.x) * 0.5f;
        float textPosY = ImGui::GetCursorPosY() - 19.0f;  // Position text over the health bar

        ImGui::SetCursorPos(ImVec2(textPosX, textPosY));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));  // White text
        ImGui::Text("%s", healthText.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::End();

    // Pop styling
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
}

// Set the height of the boss health bar
void BossHealthBar::setHeight(float height)
{
    m_height = height;
}