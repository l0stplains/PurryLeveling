#pragma once

#include <iostream>  // Debugging

#include "core/GameContext.hpp"

#include "skill/mobSkill/InfernalCurse.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/bosses/BossMob.hpp"

class DemonLord : public BossMob, public AnimatedUnit
{
public:
    DemonLord(const std::string&  name,
              const sf::Vector2f& position,
              NavigationGrid&     navGrid,  // Pass grid to base
              const GameContext&  gameContext);

    ~DemonLord() override = default;

    // Prevent copying, allow moving
    DemonLord(const DemonLord&)            = delete;
    DemonLord& operator=(const DemonLord&) = delete;
    DemonLord(DemonLord&&)                 = default;
    DemonLord& operator=(DemonLord&&)      = default;

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

    InfernalCurse m_infernalCurse;

    float m_attackRange = 32.0f;  // Example: Attack range in pixels (e.g., one tile)
};