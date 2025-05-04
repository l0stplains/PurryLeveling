#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/characters/Character.hpp"

class Berseker : public Character, public AnimatedUnit
{
public:
    Berseker(const std::string&  name,
             const sf::Vector2f& position,
             NavigationGrid&     navGrid,  // Pass grid to base
             bool                isPlayerControlled,
             const GameContext&  gameContext);

    ~Berseker() override = default;

    // Prevent copying, allow moving
    Berseker(const Berseker&)            = delete;
    Berseker& operator=(const Berseker&) = delete;
    Berseker(Berseker&&)                 = default;
    Berseker& operator=(Berseker&&)      = default;

    // --- Action Overrides ---
    void Attack(Unit&                    target,
                ActionCompletionCallback callback = nullptr,
                ActionCompletionCallback onDeath  = nullptr) override;

    void UseSkill(Unit&                    target,
                  ActionCompletionCallback callback = nullptr,
                  ActionCompletionCallback onDeath  = nullptr) override;

    int CalculateDamage(Unit& target) override;

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
                       ActionCompletionCallback onDeath);

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};