#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>  // For std::unique_ptr
#include <string>
#include <unordered_map>  // For LoadAnimations

// Core includes
#include "core/UnitManager.hpp"  // Assuming this is correctly implemented

#include "units/NavigationGrid.hpp"

// Unit includes
#include "core/ResourceManager.hpp"

#include "units/Enums.hpp"  // Include Enums for UnitAnimationType
#include "units/characters/Fighter.hpp"
#include "units/characters/Mage.hpp"

int main()
{
    // --- Window Setup ---
    const unsigned int WINDOW_WIDTH  = 1280;  // Use consistent window size
    const unsigned int WINDOW_HEIGHT = 720;
    sf::RenderWindow   window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Coba coba character");

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // Calculate the position to center the window
    int x = (desktopMode.size.x - WINDOW_WIDTH) / 2;
    int y = (desktopMode.size.y - WINDOW_HEIGHT) / 2;

    // Set the window position to the calculated position (centered)
    window.setPosition(sf::Vector2i(x, y));

    sf::Texture bgTexture;
    bgTexture.loadFromFile("resources/textures/ui/crystal_boss.jpg");

    sf::Sprite bgSprite(bgTexture);

    bgSprite.setOrigin({0, 0});
    sf::Vector2u windowSize = window.getSize();
    bgSprite.setScale({static_cast<float>(windowSize.x) / bgTexture.getSize().x,
                       static_cast<float>(windowSize.y) / bgTexture.getSize().y});
    bgSprite.setPosition({0, 0});
    window.setFramerateLimit(60);

    GameContext context;

    context.SetWindow(&window);

    ResourceManager resourceManager;

    context.SetResourceManager(&resourceManager);

    // it should be automatic by using resource parser later
    resourceManager.LoadTexture("character_centaur_idle",
                                "resources/textures/characters/Centaur/CentaurIdle.png");
    resourceManager.LoadTexture("character_centaur_walk",
                                "resources/textures/characters/Centaur/CentaurJump.png");
    resourceManager.LoadTexture("character_centaur_attack",
                                "resources/textures/characters/Centaur/CentaurAttack.png");
    resourceManager.LoadTexture("character_centaur_jump",
                                "resources/textures/characters/Centaur/CentaurJump.png");
    resourceManager.LoadTexture("character_centaur_dmg",
                                "resources/textures/characters/Centaur/CentaurDmg.png");
    resourceManager.LoadTexture("character_centaur_die",
                                "resources/textures/characters/Centaur/CentaurDie.png");
    resourceManager.LoadTexture("character_centaur_idle_shadow",
                                "resources/textures/characters/Centaur/_Shadow/"
                                "ShadowCentaurIdle.png");
    resourceManager.LoadTexture("character_centaur_walk_shadow",
                                "resources/textures/characters/Centaur/_Shadow/"
                                "ShadowCentaurJump.png");
    resourceManager.LoadTexture("character_centaur_attack_shadow",
                                "resources/textures/characters/Centaur/_Shadow/"
                                "ShadowCentaurAttack.png");
    resourceManager.LoadTexture("character_centaur_jump_shadow",
                                "resources/textures/characters/Centaur/_Shadow/"
                                "ShadowCentaurJump.png");
    resourceManager.LoadTexture("character_centaur_dmg_shadow",
                                "resources/textures/characters/Centaur/_Shadow/"
                                "ShadowCentaurDmg.png");
    resourceManager.LoadTexture("character_centaur_die_shadow",
                                "resources/textures/characters/Centaur/_Shadow/"
                                "ShadowCentaurDie.png");

    resourceManager.LoadFont("main_font", "resources/fonts/m5x7.ttf");
    // --- Navigation Grid Setup ---
    const unsigned int TILE_SIZE = 32;  // Tile size for the grid
    NavigationGrid     navGrid(WINDOW_WIDTH, WINDOW_HEIGHT, TILE_SIZE, TILE_SIZE);
    // --- Optional: Define some obstacles ---
    // Example: Make a column unwalkable
    // unsigned int gridWidth = navGrid.GetGridWidth();
    // unsigned int gridHeight = navGrid.GetGridHeight();
    // for (unsigned int y = 0; y < gridHeight; ++y) {
    //     navGrid.SetTileWalkable(gridWidth / 3, y, false); // Block column at 1/3rd width
    // }

    // --- Unit Manager ---
    UnitManager unitManager;  // Assuming UnitManager handles std::unique_ptr<Unit>

    // --- Create Hero ---
    auto fighter = std::make_unique<Mage>("Hero",
                                          sf::Vector2f(WINDOW_WIDTH / 2 - 400 * WINDOW_WIDTH / 1820,
                                                       780 * WINDOW_HEIGHT / 1024),  // Position
                                          navGrid,  // Pass navigation grid
                                          true,     // Set as player controlled
                                          context);
    fighter->SetShowUI(true);
    fighter->SetMoveSpeed(200);
    // fighter->SetControlledByPlayer(true); // No longer needed - done in constructor

    // Scale the hero slightly larger (Example)
    fighter->SetScale(8.f, 8.f);

    // Store the fighter ID for reference
    unsigned int heroId = fighter->GetId();

    // Add fighter to manager
    unitManager.AddUnit(std::move(fighter));

    // --- Create an Enemy ---
    auto enemy = std::make_unique<Mage>("Enemy",
                                        sf::Vector2f(WINDOW_WIDTH / 2 + 400 * WINDOW_WIDTH / 1820,
                                                     780 * WINDOW_HEIGHT / 1024),  // Position
                                                                                   // etc.)
                                        navGrid,  // Pass navigation grid
                                        false,    // Not player controlled
                                        context);

    enemy->SetDirection(Direction::WEST);
    enemy->SetShowUI(true);
    enemy->SetMoveSpeed(150);
    // Load enemy animations (using the same elf data for this example)

    // Store the enemy ID for reference
    unsigned int enemyId = enemy->GetId();
    enemy->SetScale(8.f, 8.f);

    // Add enemy to manager
    unitManager.AddUnit(std::move(enemy));

    // --- Z-Order Example ---
    // Make the hero appear in front (Assuming UnitManager uses Z-order)
    Unit* heroPtr  = unitManager.GetUnit(heroId);  // Get base pointer
    Unit* enemyPtr = unitManager.GetUnit(enemyId);
    if (heroPtr)
    {
        // Cast to AnimatedUnit to set Z-order (ZOrder is in AnimatedUnit)
        AnimatedUnit* animatedHero = dynamic_cast<AnimatedUnit*>(heroPtr);
        if (animatedHero)
            animatedHero->SetZOrder(1);  // Higher value = more in front
    }
    if (enemyPtr)
    {
        AnimatedUnit* animatedEnemy = dynamic_cast<AnimatedUnit*>(enemyPtr);
        if (animatedEnemy)
            animatedEnemy->SetZOrder(0);  // Lower value = further back
    }
    // unitManager.BringUnitToFront(heroId); // If manager has its own sorting logic

    // --- Main Game Loop ---
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();

        // --- Event Handling ---
        while (auto eventOpt = window.pollEvent())
        {
            sf::Event event = *eventOpt;  // Dereference the optional

            unitManager.ProcessEvent(event);
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            {
                // Let the manager handle dispatching input to the correct unit

                // --- Specific Actions on Key Press (like Attack) ---
                if (keyPressed->code == sf::Keyboard::Key::Space)  // Use Key enum
                {
                    // Make hero attack the enemy
                    // GetUnitOfType might be safer if you *know* they are Fighters
                    Mage*         heroAttackPtr = unitManager.GetUnitOfType<Mage>(heroId);
                    AnimatedUnit* enemyTargetPtr =
                        unitManager.GetUnitOfType<AnimatedUnit>(enemyId);  // Target can be base
                                                                           // Unit

                    if (heroAttackPtr && enemyTargetPtr && enemyTargetPtr->IsActive())
                    {
                        std::cout << "[Input] Hero ordered to attack Enemy!" << std::endl;
                        heroAttackPtr->Attack(*enemyTargetPtr, []() {
                            // This lambda callback runs when the ATTACK ACTION is complete
                            // (e.g., after the animation finishes and damage is potentially dealt)
                            std::cout << "[Callback] Hero attack action finished!" << std::endl;
                        });
                    }
                    else if (!enemyTargetPtr || !enemyTargetPtr->IsActive())
                    {
                        std::cout << "[Input] Cannot attack - Enemy is not valid or not active."
                                  << std::endl;
                    }
                }
                // --- Example: Add EXP on 'X' key ---
                else if (keyPressed->code == sf::Keyboard::Key::X)
                {
                    Character* heroChar = unitManager.GetUnitOfType<Character>(heroId);
                    if (heroChar)
                    {
                        heroChar->AddExp(50);  // Give 50 EXP
                    }
                }
                // --- Example: Damage enemy on 'C' key ---
                else if (keyPressed->code == sf::Keyboard::Key::C)
                {
                    AnimatedUnit* enemyAU = unitManager.GetUnitOfType<AnimatedUnit>(enemyId);
                    if (enemyAU && enemyAU->IsActive())
                    {
                        enemyAU->TakeDamage(25, []() {
                            std::cout << "[Callback] Enemy damage anim finished." << std::endl;
                        });
                    }
                }
            }
            // --- Optional: Handle mouse clicks for movement/targeting ---
            else if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseClick->button == sf::Mouse::Button::Left)
                {
                    AnimatedUnit* heroAnimatedUnit = unitManager.GetUnitOfType<AnimatedUnit>(heroId);
                    if (heroAnimatedUnit && heroAnimatedUnit->IsPlayerControlled())
                    {
                        sf::Vector2f targetPos = window.mapPixelToCoords(
                            {mouseClick->position.x, mouseClick->position.y});
                        std::cout << "[Input] Hero ordered to move to (" << targetPos.x << ", "
                                  << targetPos.y << ")" << std::endl;
                        heroAnimatedUnit->Move(targetPos, []() {
                            std::cout << "[Callback] Hero movement finished." << std::endl;
                        });
                    }
                }
            }
        }

        // --- Update ---
        // Update all units (handles movement, animation, AI logic if implemented)
        unitManager.Update(dt);

        // --- Draw ---
        window.clear(sf::Color(200, 200, 200));  // Clear with dark grey
        window.draw(bgSprite);

        // Draw units (Assuming UnitManager draws them in correct order - e.g., sorted by Z)
        unitManager.Draw(window);

        // Draw UI elements (e.g., health bars) over units
        unitManager.DrawUI(window);  // Add DrawUI to UnitManager if needed

        window.display();
    }

    return 0;
}