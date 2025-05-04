#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "units/AnimatedUnit.hpp"
#include "units/characters/Character.hpp"

class Mage : public Character, public AnimatedUnit
{
public:
    Mage(const std::string&  name,
         const sf::Vector2f& position,
         NavigationGrid&     navGrid,  // Pass grid to base
         bool                isPlayerControlled,
         const GameContext&  gameContext);

    ~Mage() override = default;

    // Prevent copying, allow moving
    Mage(const Mage&)            = delete;
    Mage& operator=(const Mage&) = delete;
    Mage(Mage&&)                 = default;
    Mage& operator=(Mage&&)      = default;

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
    vector<unsigned int> m_summons;  // List of summoned units
    int                  m_summonedUnits = 1;

    int m_extraMana = 10;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};