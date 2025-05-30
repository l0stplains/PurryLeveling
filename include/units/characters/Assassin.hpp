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
    void Attack(Unit&                    target,
                ActionCompletionCallback callback = nullptr,
                ActionCompletionCallback onDeath  = nullptr) override;

    bool UseSkill(Unit&                    target,
                  ActionCompletionCallback callback = nullptr,
                  ActionCompletionCallback onDeath  = nullptr) override;

    void TakeDamage(int                      damage,
                    ActionCompletionCallback callback = nullptr,
                    ActionCompletionCallback onDeath  = nullptr) override;

    void SetLevel(int level) override;

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

    float m_criticalHitChance     = 0.25f;
    float m_criticalHitMultiplier = 2.0f;

    float m_agilityMultiplier = 1.0f;

    float m_attackRange = 48.0f;
};