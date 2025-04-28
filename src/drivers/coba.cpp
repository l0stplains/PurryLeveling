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

int main()
{
    // --- Window Setup ---
    const unsigned int WINDOW_WIDTH  = 1280;  // Use consistent window size
    const unsigned int WINDOW_HEIGHT = 720;
    sf::RenderWindow   window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Coba coba character");
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

    // --- Prepare Animation Data (Example for Centaur) ---
    // You'll need to adjust frame counts, durations, and frame size based on your actual assets
    sf::Vector2i elfFrameSize(32, 32);  // Example frame size

    std::unordered_map<UnitAnimationType, std::string> elfTexturePaths = {
        {UnitAnimationType::IDLE, "character_centaur_idle"},
        {UnitAnimationType::WALK, "character_centaur_jump"},
        {UnitAnimationType::ATTACK, "character_centaur_attack"},
        {UnitAnimationType::JUMP, "character_centaur_jump"},  // Assuming Jump exists
        {UnitAnimationType::DAMAGE, "character_centaur_dmg"},
        {UnitAnimationType::DIE, "character_centaur_die"}  // Add SKILL if you have a specific
                                                           // texture
    };
    std::unordered_map<UnitAnimationType, std::string> elfShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_centaur_idle_shadow"},
        {UnitAnimationType::WALK, "character_centaur_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_centaur_attack_shadow"},
        {UnitAnimationType::JUMP, "character_centaur_jump_shadow"},  // Assuming Jump exists
        {UnitAnimationType::DAMAGE, "character_centaur_dmg_shadow"},
        {UnitAnimationType::DIE, "character_centaur_die_shadow"}  // Add SKILL if you have a
                                                                  // specific texture
    };
    // Example frame counts (adjust these!)
    std::unordered_map<UnitAnimationType, int> elfFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},  // Jump might be single frame or need more logic
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    // Example durations (in seconds, adjust these!)
    std::unordered_map<UnitAnimationType, float> elfDurationPerAnim = {
        {UnitAnimationType::IDLE, 3.2f},  // Longer idle loop
        {UnitAnimationType::WALK, 0.8f},
        {UnitAnimationType::ATTACK, 0.4f},
        {UnitAnimationType::JUMP, 0.4f},
        {UnitAnimationType::DAMAGE, 0.4f},
        {UnitAnimationType::DIE, 1.2f}};

    // Example looping status (Idle/Walk usually loop)
    std::unordered_map<UnitAnimationType, bool> elfLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},  // Attack usually plays once
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}  // Die plays once
    };

    std::unordered_map<UnitAnimationType, bool> elfDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> elfDefaultRows = {{UnitAnimationType::DIE, 0}};

    // --- Create Hero ---
    auto fighter =
        std::make_unique<Fighter>("Hero",
                                  sf::Vector2f(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.5f),  // Position
                                  navGrid,  // Pass navigation grid
                                  true,     // Set as player controlled
                                  context);
    fighter->SetShowUI(true);
    // Load hero animations using the new method
    if (!fighter->LoadAnimations(elfTexturePaths,
                                 elfFrameSize,
                                 elfFramesPerAnim,
                                 elfDurationPerAnim,
                                 elfLoopingAnims,
                                 elfDirectionalAnims,
                                 elfDefaultRows,
                                 elfShadowTexturePaths))
    {
        std::cerr << "Error loading Hero animations!" << std::endl;
        return 1;  // Exit if loading failed
    }
    // fighter->SetControlledByPlayer(true); // No longer needed - done in constructor

    // Scale the hero slightly larger (Example)
    fighter->SetScale(6.f, 6.f);

    // Store the fighter ID for reference
    unsigned int heroId = fighter->GetId();

    // Add fighter to manager
    unitManager.AddUnit(std::move(fighter));

    // --- Create an Enemy ---
    auto enemy = std::make_unique<Fighter>("Enemy",
                                           sf::Vector2f(WINDOW_WIDTH * 0.7f, WINDOW_HEIGHT * 0.5f),
                                           navGrid,  // Pass navigation grid
                                           false,    // Not player controlled
                                           context);

    enemy->SetShowUI(true);
    // Load enemy animations (using the same elf data for this example)
    if (!enemy->LoadAnimations(elfTexturePaths,
                               elfFrameSize,
                               elfFramesPerAnim,
                               elfDurationPerAnim,
                               elfLoopingAnims,
                               elfDirectionalAnims,
                               elfDefaultRows,
                               elfShadowTexturePaths))
    {
        std::cerr << "Error loading Enemy animations!" << std::endl;
        return 1;  // Exit if loading failed
    }

    // Store the enemy ID for reference
    unsigned int enemyId = enemy->GetId();
    enemy->SetScale(6.f, 6.f);

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
                    Fighter*      heroAttackPtr = unitManager.GetUnitOfType<Fighter>(heroId);
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

        // Draw units (Assuming UnitManager draws them in correct order - e.g., sorted by Z)
        unitManager.Draw(window);

        // Draw UI elements (e.g., health bars) over units
        unitManager.DrawUI(window);  // Add DrawUI to UnitManager if needed

        window.display();
    }

    return 0;
}