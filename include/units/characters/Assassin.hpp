#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/characters/Character.hpp"

class Assassin : public Character, public AnimatedUnit
{
public:
    Assassin(const std::string&  name,
             const sf::Vector2f& position,
             NavigationGrid&     navGrid,  // Pass grid to base
             bool                isPlayerControlled,
             const GameContext&  gameContext);

    ~Assassin() override = default;

    // Prevent copying, allow moving
    Assassin(const Assassin&)            = delete;
    Assassin& operator=(const Assassin&) = delete;
    Assassin(Assassin&&)                 = default;
    Assassin& operator=(Assassin&&)      = default;

    // --- Action Overrides ---
    void Attack(Unit& target, ActionCompletionCallback callback = nullptr) override;

    void UseSkill(int skillId, ActionCompletionCallback callback = nullptr) override;

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