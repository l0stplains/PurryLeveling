#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "skill/mobSkill/WitheringTouch.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/bosses/BossMob.hpp"

class Lich : public BossMob, public AnimatedUnit
{
public:
    Lich(const std::string&  name,
         const sf::Vector2f& position,
         NavigationGrid&     navGrid,  // Pass grid to base
         const GameContext&  gameContext);

    ~Lich() override = default;

    // Prevent copying, allow moving
    Lich(const Lich&)            = delete;
    Lich& operator=(const Lich&) = delete;
    Lich(Lich&&)                 = default;
    Lich& operator=(Lich&&)      = default;

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

    WitheringTouch m_witheringTouch;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};