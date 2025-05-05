#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "skill/mobSkill/AcidityCorrosion.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/basics/BasicMob.hpp"

class Slime : public BasicMob, public AnimatedUnit
{
public:
    Slime(const std::string&  name,
          const sf::Vector2f& position,
          NavigationGrid&     navGrid,  // Pass grid to base
          const GameContext&  gameContext);

    ~Slime() override = default;

    // Prevent copying, allow moving
    Slime(const Slime&)            = delete;
    Slime& operator=(const Slime&) = delete;
    Slime(Slime&&)                 = default;
    Slime& operator=(Slime&&)      = default;

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

    AcidityCorrosion m_acidityCorrossion;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};