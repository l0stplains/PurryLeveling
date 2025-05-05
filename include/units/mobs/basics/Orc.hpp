#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "skill/mobSkill/BrutalStrike.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/basics/BasicMob.hpp"

class Orc : public BasicMob, public AnimatedUnit
{
public:
    Orc(const std::string&  name,
        const sf::Vector2f& position,
        NavigationGrid&     navGrid,  // Pass grid to base
        const GameContext&  gameContext);

    ~Orc() override = default;

    // Prevent copying, allow moving
    Orc(const Orc&)            = delete;
    Orc& operator=(const Orc&) = delete;
    Orc(Orc&&)                 = default;
    Orc& operator=(Orc&&)      = default;

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

    BrutalStrike m_brutalStrike;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};