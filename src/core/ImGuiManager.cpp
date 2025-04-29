#include "core/ImGuiManager.hpp"

ImGuiManager::ImGuiManager() : m_initialized(false) {}
ImGuiManager::~ImGuiManager()
{
    if (m_initialized)
    {
        Shutdown();
    }
}
void ImGuiManager::Init(sf::RenderWindow& window)
{
    if (!m_initialized)
    {
        ImGui::SFML::Init(window);
        SetDarkTheme();
        m_initialized = true;
        m_window      = &window;
    }
}
void ImGuiManager::ProcessEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (m_initialized)
    {
        ImGui::SFML::ProcessEvent(window, event);
    }
}

void ImGuiManager::NewFrame()
{
    if (m_initialized)
    {
        ImGui::SFML::Update(*m_window, m_deltaClock.restart());
    }
}
void ImGuiManager::Render(sf::RenderWindow& window)
{
    if (m_initialized)
    {
        ImGui::SFML::Render(window);
    }
}
void ImGuiManager::Shutdown()
{
    if (m_initialized)
    {
        ImGui::SFML::Shutdown();
        m_initialized = false;
    }
}
void ImGuiManager::SetDarkTheme()
{
    ImGuiStyle& style                   = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]         = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]     = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);
    style.Colors[ImGuiCol_ChildBg]      = ImVec4(0.12f, 0.12f, 0.12f, 0.70f);
    style.Colors[ImGuiCol_PopupBg]      = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);

    style.Colors[ImGuiCol_Border]       = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.20f, 0.20f, 0.22f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.30f, 0.30f, 0.30f, 0.67f);

    style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);

    style.Colors[ImGuiCol_Button]        = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.80f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.39f, 0.53f, 0.95f, 1.00f);
}
void ImGuiManager::SetLightTheme()
{
    ImGuiStyle& style                   = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]         = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]     = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    style.Colors[ImGuiCol_ChildBg]      = ImVec4(0.92f, 0.92f, 0.92f, 0.80f);
    style.Colors[ImGuiCol_PopupBg]      = ImVec4(0.92f, 0.92f, 0.92f, 0.80f);

    style.Colors[ImGuiCol_Border]       = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);

    style.Colors[ImGuiCol_FrameBg]        = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

    style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);

    style.Colors[ImGuiCol_Button]        = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
}
void ImGuiManager::SetCustomTheme(const ImVec4& textColor,
                                  const ImVec4& headColor,
                                  const ImVec4& areaColor,
                                  const ImVec4& bodyColor,
                                  const ImVec4& popupColor)
{
    ImGuiStyle& style                   = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]         = textColor;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(textColor.x, textColor.y, textColor.z, 0.58f);

    style.Colors[ImGuiCol_WindowBg] = bodyColor;
    style.Colors[ImGuiCol_ChildBg]  = bodyColor;
    style.Colors[ImGuiCol_PopupBg]  = popupColor;

    style.Colors[ImGuiCol_Border]       = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    style.Colors[ImGuiCol_FrameBg]        = areaColor;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
    style.Colors[ImGuiCol_FrameBgActive]  = headColor;

    style.Colors[ImGuiCol_TitleBg]          = ImVec4(headColor.x, headColor.y, headColor.z, 0.80f);
    style.Colors[ImGuiCol_TitleBgActive]    = headColor;
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(headColor.x, headColor.y, headColor.z, 0.40f);

    style.Colors[ImGuiCol_Button]        = ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
    style.Colors[ImGuiCol_ButtonActive]  = headColor;
}