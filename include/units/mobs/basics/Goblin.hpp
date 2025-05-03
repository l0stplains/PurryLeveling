#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "skill/mobSkill/SneakyStab.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/basics/BasicMob.hpp"

class Goblin : public BasicMob, public AnimatedUnit
{
public:
    Goblin(const std::string&  name,
           const sf::Vector2f& position,
           NavigationGrid&     navGrid,  // Pass grid to base
           const GameContext&  gameContext);

    ~Goblin() override = default;

    // Prevent copying, allow moving
    Goblin(const Goblin&)            = delete;
    Goblin& operator=(const Goblin&) = delete;
    Goblin(Goblin&&)                 = default;
    Goblin& operator=(Goblin&&)      = default;

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

    SneakyStab m_sneakyStab;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};