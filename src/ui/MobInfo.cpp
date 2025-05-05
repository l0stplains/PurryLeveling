#include "ui/MobInfo.hpp"

MobInfo::MobInfo(GameContext& gameContext, float windowWidth, float windowHeight)
    : m_windowWidth(windowWidth),
      m_windowHeight(windowHeight),
      m_gameContext(gameContext),
      m_titleColor(0.95f, 0.9f, 0.6f, 1.0f),            // Gold-ish for title
      m_statsColor(0.8f, 0.8f, 0.9f, 1.0f),             // Light blue-ish for stat names
      m_valueColor(1.0f, 1.0f, 1.0f, 1.0f),             // White for values
      m_backgroundColorDark(0.1f, 0.1f, 0.15f, 1.0f),   // Dark blue-ish background
      m_backgroundColorLight(0.2f, 0.2f, 0.25f, 1.0f),  // Slightly lighter blue for contrast
      m_borderColor(0.5f, 0.5f, 0.7f, 1.0f),            // Purple-ish border
      m_healthBarColor(0.9f, 0.2f, 0.2f, 0.9f),         // Red for health bar
      m_healthBarBgColor(0.2f, 0.1f, 0.1f, 0.6f),       // Darker red for health background
      m_manaBarColor(0.2f, 0.4f, 0.8f, 0.9f),           // Blue for mana bar
      m_manaBarBgColor(0.1f, 0.15f, 0.3f, 0.6f)         // Darker blue for mana background
{
}

void MobInfo::render(const std::vector<unsigned int>& mobIds, bool isBossRoom)
{
    // Don't show mob info if it's a boss room or there are no mobs
    if (mobIds.empty() || isBossRoom)
        return;

    // Set window position and size in the bottom right corner
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 windowPos(viewport->WorkPos.x + viewport->WorkSize.x - m_windowWidth - 10,  // Bottom right
                     viewport->WorkPos.y + viewport->WorkSize.y - m_windowHeight - 10);

    // Set window position and size
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiCond_Always);
    
    // Window flags - add NoTitleBar to remove header
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | 
                             ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoTitleBar; // Added NoTitleBar flag
    
    // Style customization - semi-transparent background
    ImVec4 bgColor = m_backgroundColorDark;
    bgColor.w = 0.8f; // Reduce opacity to 80%
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Border, m_borderColor);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f); // Round corners
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10)); // Consistent padding
    
    // Begin mob info window
    if (ImGui::Begin("Mob Info", nullptr, flags))
    {
        // Title
        ImGui::PushStyleColor(ImGuiCol_Text, m_titleColor);
        ImGui::TextWrapped("Enemy Units");
        ImGui::PopStyleColor(); // Text color
        
        ImGui::Separator();
        
        // Create a child window with scrolling for mobs list
        ImGui::BeginChild("MobsList", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        
        // Add some style for better visualization of bars
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f); // Increase rounding for frames
        
        // Iterate through all mobs
        for (auto id : mobIds)
        {
            // Get the animated unit and mob from the unit manager
            AnimatedUnit* animatedUnit = m_gameContext.GetUnitManager()->GetUnitOfType<AnimatedUnit>(id);
            Mob* mob = m_gameContext.GetUnitManager()->GetUnitOfType<Mob>(id);
            
            if (!animatedUnit || !mob || !animatedUnit->IsActive())
                continue;

            // Mob name and level
            ImGui::PushStyleColor(ImGuiCol_Text, m_statsColor);
            ImGui::Text("%s (Lvl %d)", animatedUnit->GetName().c_str(), mob->GetLevel());
            ImGui::PopStyleColor();
            
            // Health bar with text inside
            float healthRatio = static_cast<float>(animatedUnit->GetHealth()) / 
                               static_cast<float>(animatedUnit->GetMaxHealth());
            
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, m_healthBarColor);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, m_healthBarBgColor); // Add background color
            
            // Create HP string to overlay on bar
            char hpOverlay[32];
            snprintf(hpOverlay, sizeof(hpOverlay), "HP: %d/%d", 
                    animatedUnit->GetHealth(), animatedUnit->GetMaxHealth());
            
            // Reduced height progress bar
            ImGui::ProgressBar(healthRatio, ImVec2(-1, 16), ""); // Slightly shorter than character's
            
            // Calculate position for text overlay
            ImVec2 barMin = ImGui::GetItemRectMin();
            ImVec2 barMax = ImGui::GetItemRectMax();
            ImVec2 textSize = ImGui::CalcTextSize(hpOverlay);
            ImVec2 textPos = ImVec2(
                barMin.x + (barMax.x - barMin.x - textSize.x) * 0.5f,
                barMin.y + (barMax.y - barMin.y - textSize.y) * 0.5f
            );
            
            // Draw text overlay on progress bar
            ImGui::GetWindowDrawList()->AddText(
                textPos, 
                IM_COL32(255, 255, 255, 255), // White text
                hpOverlay
            );
            
            ImGui::PopStyleColor(2); // Pop health bar colors
            
            // Mana bar with text inside
            float manaRatio = static_cast<float>(animatedUnit->GetCurrentMana()) / 
                             static_cast<float>(animatedUnit->GetMaxMana());
            
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, m_manaBarColor);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, m_manaBarBgColor); // Add background color
            
            // Create MP string to overlay on bar
            char mpOverlay[32];
            snprintf(mpOverlay, sizeof(mpOverlay), "MP: %d/%d", 
                    animatedUnit->GetCurrentMana(), animatedUnit->GetMaxMana());
            
            // Reduced height progress bar
            ImGui::ProgressBar(manaRatio, ImVec2(-1, 16), ""); // Slightly shorter
            
            // Calculate position for text overlay
            barMin = ImGui::GetItemRectMin();
            barMax = ImGui::GetItemRectMax();
            textSize = ImGui::CalcTextSize(mpOverlay);
            textPos = ImVec2(
                barMin.x + (barMax.x - barMin.x - textSize.x) * 0.5f,
                barMin.y + (barMax.y - barMin.y - textSize.y) * 0.5f
            );
            
            // Draw text overlay on progress bar
            ImGui::GetWindowDrawList()->AddText(
                textPos, 
                IM_COL32(255, 255, 255, 255), // White text
                mpOverlay
            );
            
            ImGui::PopStyleColor(2); // Pop mana bar colors
            
            // Add a bit of spacing between mobs
            ImGui::Spacing();
            if (mobIds.size() > 1)
                ImGui::Separator();
        }
        
        ImGui::PopStyleVar(); // Pop frame rounding
        ImGui::EndChild(); // End scrollable area
    }
    ImGui::End();
    
    // Pop all style colors and vars that were pushed
    ImGui::PopStyleVar(2); // Pop window rounding and padding
    ImGui::PopStyleColor(2); // Pop window styling colors
}