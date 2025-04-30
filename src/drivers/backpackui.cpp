#include <SFML/Graphics.hpp>

#include "ui/InventorySystem.hpp"

#include "imgui-SFML.h"
#include "imgui.h"
#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
int main()
{
    // Create SFML window
    const unsigned int WINDOW_WIDTH  = 1280;
    const unsigned int WINDOW_HEIGHT = 720;
    sf::RenderWindow   window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Inventory System");
    window.setFramerateLimit(60);

    sf::Texture m_backgroundTexture;
    if (!m_backgroundTexture.loadFromFile("resources/textures/ui/backpack_background.png"))
    {
        return -1;  // Handle error
    }
    sf::Vector2u windowSize = window.getSize();
    sf::Sprite   m_backgroundSprite(m_backgroundTexture);
    m_backgroundSprite.setTexture(m_backgroundTexture);

    m_backgroundSprite.setOrigin({0, 0});
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    // Initialize ImGui
    ImGui::SFML::Init(window);

    // Create inventory system
    InventorySystem inventory;

    // Main loop
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        // Event Handling
        while (auto eventOpt = window.pollEvent())
        {
            sf::Event event = *eventOpt;
            ImGui::SFML::ProcessEvent(window, event);

            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());

        // Render inventory
        inventory.render();

        // Clear window and draw ImGui
        window.clear(sf::Color(64, 64, 64));
        window.draw(m_backgroundSprite);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}