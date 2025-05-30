#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/characters/Character.hpp"

/**
 * @brief Concrete implementation of a Fighter Character.
 */
class Fighter : public Character, public AnimatedUnit
{
public:
    Fighter(const std::string&  name,
            const sf::Vector2f& position,
            NavigationGrid&     navGrid,  // Pass grid to base
            bool                isPlayerControlled,
            const GameContext&  gameContext);

    ~Fighter() override = default;

    // Prevent copying, allow moving
    Fighter(const Fighter&)            = delete;
    Fighter& operator=(const Fighter&) = delete;
    Fighter(Fighter&&)                 = default;
    Fighter& operator=(Fighter&&)      = default;

    // --- Action Overrides ---
    /**
     * @brief Fighter's attack implementation. Checks range, moves if necessary, deals damage.
     */
    void Attack(Unit&                    target,
                ActionCompletionCallback callback = nullptr,
                ActionCompletionCallback onDeath  = nullptr) override;

    /**
     * @brief Fighter's skill implementation.
     */
    bool UseSkill(Unit&                    target,
                  ActionCompletionCallback callback = nullptr,
                  ActionCompletionCallback onDeath  = nullptr) override;

    void TakeDamage(int                      damage,
                    ActionCompletionCallback callback = nullptr,
                    ActionCompletionCallback onDeath  = nullptr) override;

    void SetLevel(int level) override;

    int CalculateDamage(Unit& target) override;

    // void TakeDamage(int damage, ActionCompletionCallback callback = nullptr) override;

    /**
     * @brief Override RenderUI if fighter has specific visuals.
     */
    // void RenderUI(sf::RenderWindow& window) override;

    void SetBlockChance(float chance) { m_blockChance = chance; }

private:
    /**
     * @brief Helper function to perform the attack animation and damage after range check.
     */
    void PerformAttack(AnimatedUnit&            target,
                       ActionCompletionCallback callback,
                       ActionCompletionCallback onDeath);

    // --- Fighter Specific Stats ---
    float m_attackRange = 32.0f;

    float m_blockChance = 0.2f;
};