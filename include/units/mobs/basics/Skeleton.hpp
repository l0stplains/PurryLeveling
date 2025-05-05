#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "skill/mobSkill/BoneShard.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/basics/BasicMob.hpp"

class Skeleton : public BasicMob, public AnimatedUnit
{
public:
    Skeleton(const std::string&  name,
             const sf::Vector2f& position,
             NavigationGrid&     navGrid,  // Pass grid to base
             const GameContext&  gameContext);

    ~Skeleton() override = default;

    // Prevent copying, allow moving
    Skeleton(const Skeleton&)            = delete;
    Skeleton& operator=(const Skeleton&) = delete;
    Skeleton(Skeleton&&)                 = default;
    Skeleton& operator=(Skeleton&&)      = default;

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

    BoneShard m_boneShard;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};