#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/summons/Summon.hpp"

class Wildfire : public Summon, public AnimatedUnit
{
public:
    Wildfire(const std::string&  name,
             const sf::Vector2f& position,
             NavigationGrid&     navGrid,  // Pass grid to base
             bool                isPlayerControlled,
             const GameContext&  gameContext);

    ~Wildfire() override = default;

    // Prevent copying, allow moving
    Wildfire(const Wildfire&)            = delete;
    Wildfire& operator=(const Wildfire&) = delete;
    Wildfire(Wildfire&&)                 = default;
    Wildfire& operator=(Wildfire&&)      = default;

    // --- Action Overrides ---
    void Attack(Unit&                    target,
                ActionCompletionCallback callback = nullptr,
                ActionCompletionCallback onDeath  = nullptr) override;

    void UseSkill(Unit&                    target,
                  ActionCompletionCallback callback = nullptr,
                  ActionCompletionCallback onDeath  = nullptr) override;

    /**
     * @brief Override RenderUI if fighter has specific visuals.
     */
    // void RenderUI(sf::RenderWindow& window) override;

private:
    /**
     * @brief Helper function to perform the attack animation and damage after range check.
     */
    void PerformAttack(AnimatedUnit&            target,
                       ActionCompletionCallback callback,
                       ActionCompletionCallback onDeath = nullptr);

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};