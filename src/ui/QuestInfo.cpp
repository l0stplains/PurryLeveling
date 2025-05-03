#include "ui/QuestInfo.hpp"

QuestInfo::QuestInfo(GameContext& gameContext, float windowWidth, float windowHeight)
    : m_windowWidth(windowWidth),
      m_windowHeight(windowHeight),
      m_gameContext(gameContext)
{
    // Initialize any resources needed
}

void QuestInfo::render(const Dungeon& dungeon)
{
    // Get the quest from the dungeon
    const Quest& quest = dungeon.getQuest();
    
    // Get viewport dimensions for positioning (upper right)
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 windowPos(viewport->WorkPos.x + viewport->WorkSize.x - m_windowWidth - 10, // Upper right
                     viewport->WorkPos.y + 10);
    
    // Set window position and size
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiCond_Always);
    
    // Window flags to make it non-movable, non-resizable, and non-collapsable
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
    
    // Use a compact style for this smaller window
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.6f));  // Dark grey
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));  // Light grey border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);  // Rounded corners
    
    // Create the window
    if (ImGui::Begin("Quest Info", nullptr, flags))
    {
        // Quest Title
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Quest");
        ImGui::Separator();

        if (!quest.isValid())
        {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No Quest Available");
        }
        else
        {
            // Quest Type
            std::string questTypeStr = (quest.getType() == QuestType::KILL) ? "Kill Mobs" : "Deal Damage";
            ImGui::Text("Type: %s", questTypeStr.c_str());
            
            // Progress Bar
            float progressRatio = quest.getTargetCount() > 0 
                ? static_cast<float>(quest.getCurrentProgress()) / static_cast<float>(quest.getTargetCount()) 
                : 0.0f;
            
            // Clamp progress ratio to prevent overflow rendering
            if (progressRatio > 1.0f) progressRatio = 1.0f;
            
            ImGui::Text("Progress: %d/%d", quest.getCurrentProgress(), quest.getTargetCount());
            ImGui::ProgressBar(progressRatio, ImVec2(-1, 0), "");
            
            // Completed Status with checkbox style
            ImGui::Text("Status:");
            ImGui::SameLine();
            if (quest.getIsCompleted())
            {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Completed");
            }
            else
            {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "□ In Progress");
            }
            
            // Rewards section
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Rewards:");
            ImGui::Text("Gold: %d", quest.getGoldReward());
            ImGui::Text("EXP: %d", quest.getExpReward());
    
            if (!quest.getItemRewardId().empty())
            {
                ImGui::Text("Item: %s", quest.getItemRewardId().c_str());
            }
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%s", quest.getDungeonRank().c_str());
        }
        
    }
    ImGui::End();
    
    // Pop the style changes
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(2);
}