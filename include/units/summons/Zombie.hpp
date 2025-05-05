#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/summons/Summon.hpp"

class Zombie : public Summon, public AnimatedUnit
{
public:
    Zombie(const std::string&  name,
           const sf::Vector2f& position,
           NavigationGrid&     navGrid,  // Pass grid to base
           bool                isPlayerControlled,
           const GameContext&  gameContext);

    ~Zombie() override = default;

    // Prevent copying, allow moving
    Zombie(const Zombie&)            = delete;
    Zombie& operator=(const Zombie&) = delete;
    Zombie(Zombie&&)                 = default;
    Zombie& operator=(Zombie&&)      = default;

    // --- Action Overrides ---
    void Attack(Unit&                    target,
                ActionCompletionCallback callback = nullptr,
                ActionCompletionCallback onDeath  = nullptr) override;

    bool UseSkill(Unit&                    target,
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