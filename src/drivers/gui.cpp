#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

int main()
{
    // 1) Create SFML window using Vector2u constructor for SFML 3
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Purry Leveling");
    window.setFramerateLimit(60);

    // 2) Initialize ImGui-SFML
    bool initSuccess = ImGui::SFML::Init(window);
    if (!initSuccess)
    {
        // Handle initialization failure
        return -1;
    }

    // Clock for delta time
    sf::Clock deltaClock;

    // 3) Main loop
    while (window.isOpen())
    {
        // Event handling for SFML 3
        std::optional<sf::Event> eventOpt;

        while ((eventOpt = window.pollEvent()))
        {
            const sf::Event& event = *eventOpt;

            // Process event with window and event parameters
            ImGui::SFML::ProcessEvent(window, event);

            // In SFML 3, we need to check the event variant type differently
            // Use the `is<>()` and `as<>()` methods for event type checking
            if (eventOpt->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // 3b) Update ImGui state
        ImGui::SFML::Update(window, deltaClock.restart());

        // 3c) (TODO) Your game update logic here
        //     e.g. engine.update(dt);

        // 3d) Start a new ImGui frame; demo window as placeholder
        ImGui::Begin("Hello, Purry!");
        ImGui::Text("Welcome to Purry Leveling!");

        static bool showDemo = false;
        ImGui::Checkbox("Show ImGui Demo", &showDemo);

        if (showDemo)
        {
            ImGui::ShowDemoWindow(&showDemo);
        }

        ImGui::End();

        // 3e) Clear, render game & ImGui, display
        window.clear(sf::Color(30, 30, 30));
        //     e.g. engine.render(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    // 4) Shutdown
    ImGui::SFML::Shutdown();
    return 0;
}
