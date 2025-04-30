#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

/**
 * @class ImGuiManager
 * @brief Manages ImGui integration with SFML
 *
 * This class handles the initialization, event processing,
 * rendering, and shutdown of ImGui within an SFML application.
 * It also provides methods for customizing the ImGui theme.
 */
class ImGuiManager
{
public:
    /**
     * @brief Default constructor
     */
    ImGuiManager();

    /**
     * @brief Destructor
     */
    ~ImGuiManager();

    // Core ImGui methods

    /**
     * @brief Initializes ImGui with the given SFML window
     *
     * @param window The SFML render window to associate with ImGui
     */
    void Init(sf::RenderWindow& window);

    /**
     * @brief Processes SFML events for ImGui
     *
     * @param event The SFML event to process
     * @param window The SFML render window
     */
    void ProcessEvent(const sf::Event& event, sf::RenderWindow& window);

    /**
     * @brief Starts a new ImGui frame
     */
    void NewFrame();

    /**
     * @brief Renders ImGui content to the given SFML window
     *
     * @param window The SFML render window to render to
     */
    void Render(sf::RenderWindow& window);

    /**
     * @brief Shuts down ImGui and cleans up resources
     */
    void Shutdown();

    // UI theme customization

    /**
     * @brief Sets the ImGui theme to dark
     */
    void SetDarkTheme();

    /**
     * @brief Sets the ImGui theme to light
     */
    void SetLightTheme();

    /**
     * @brief Sets a custom ImGui theme with specified colors
     *
     * @param textColor Text color
     * @param headColor Header color
     * @param areaColor Area color
     * @param bodyColor Body color
     * @param popupColor Popup color
     */
    void SetCustomTheme(const ImVec4& textColor,
                        const ImVec4& headColor,
                        const ImVec4& areaColor,
                        const ImVec4& bodyColor,
                        const ImVec4& popupColor);

private:
    bool              m_initialized;  ///< Flag indicating if ImGui is initialized
    sf::Clock         m_deltaClock;   ///< Clock for measuring frame time
    sf::RenderWindow* m_window;       ///< Pointer to the associated SFML render window
};