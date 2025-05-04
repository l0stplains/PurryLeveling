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
    void Attack(Unit&                    target,
                ActionCompletionCallback callback = nullptr,
                ActionCompletionCallback onDeath  = nullptr) override;

    void UseSkill(Unit&                    target,
                  ActionCompletionCallback callback = nullptr,
                  ActionCompletionCallback onDeath  = nullptr) override;

    void KillSummons(ActionCompletionCallback callback = nullptr);

    /**
     * @brief Override RenderUI if fighter has specific visuals.
     */
    // void RenderUI(sf::RenderWindow& window) override;

    const std::vector<unsigned int>& GetSummons() const;
    float                            GetSummonChance() const;
    int                              GetSummonedUnits() const;
    float                            GetLifestealPercentage() const;
    void                             SetSummonChance(float chance);
    void                             SetSummonedUnits(int units);
    void                             SetLifestealPercentage(float percentage);

private:
    vector<unsigned int> m_summons;  // List of summoned units
    int                  m_summonedUnits       = 1;
    float                m_lifestealPercentage = 0.1f;  // 10% lifesteal
    float                m_summonChance        = 0.8f;  // 80/20, i like gambling
    float                m_attackRange         = 32.0f;
};