#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/characters/Character.hpp"

class Necromancer : public Character, public AnimatedUnit
{
public:
    Necromancer(const std::string&  name,
                const sf::Vector2f& position,
                NavigationGrid&     navGrid,  // Pass grid to base
                bool                isPlayerControlled,
                const GameContext&  gameContext);

    ~Necromancer() override = default;

    // Prevent copying, allow moving
    Necromancer(const Necromancer&)            = delete;
    Necromancer& operator=(const Necromancer&) = delete;
    Necromancer(Necromancer&&)                 = default;
    Necromancer& operator=(Necromancer&&)      = default;

    // --- Action Overrides ---
    void Attack(Unit& target, ActionCompletionCallback callback = nullptr) override;

    void UseSkill(Unit& target, ActionCompletionCallback callback = nullptr) override;

    /**
     * @brief Override RenderUI if fighter has specific visuals.
     */
    // void RenderUI(sf::RenderWindow& window) override;

private:
    /**
     * @brief Helper function to perform the attack animation and damage after range check.
     */
    void PerformAttack(AnimatedUnit& target, ActionCompletionCallback callback);

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};