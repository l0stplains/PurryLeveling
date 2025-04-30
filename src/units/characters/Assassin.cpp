#include "units/characters/Assassin.hpp"

#include <cmath>     // For sqrt
#include <iostream>  // For debug output

Assassin::Assassin(const std::string&  name,
                   const sf::Vector2f& position,
                   NavigationGrid&     navGrid,
                   bool                isPlayerControlled,
                   const GameContext&  gameContext)
    : Unit(name),  // ← must initialize the virtual base
      Character(name, position),
      AnimatedUnit(name, position, navGrid, isPlayerControlled, gameContext)
{
    // Fighter-specific stat overrides
    m_maxHealth = 100;
    SetHealth(m_maxHealth);
    m_maxMana = 100;
    SetCurrentMana(m_maxMana);
    m_attackDamage = 25;
    m_healthRegen  = 3;
    m_manaRegen    = 3;
    m_moveSpeed    = 180.f;
    m_attackRange  = 48.f;
}

void Assassin::Attack(Unit& target, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {  // Check self and target state
        if (callback)
            callback();  // Cannot attack, call callback immediately
        return;
    }

    // TODO: Check attack cooldown
    // if (m_currentAttackCooldown > 0) { /* Still on cooldown */ if (callback) callback(); return;
    // }

    AnimatedUnit* animatedTarget = dynamic_cast<AnimatedUnit*>(&target);

    sf::Vector2f vectorToTarget = animatedTarget->GetPosition() - m_position;
    float        distanceToTarget =
        std::sqrt(vectorToTarget.x * vectorToTarget.x + vectorToTarget.y * vectorToTarget.y);

    if (distanceToTarget > m_attackRange)
    {
        // Target out of range, move closer first
        // Calculate a position just within attack range
        sf::Vector2f direction       = vectorToTarget / distanceToTarget;
        sf::Vector2f positionInRange = animatedTarget->GetPosition() -
                                       direction * (m_attackRange * 0.9f);  // Move 90% of the way

        std::cout << GetName() << " moving to attack " << target.GetName() << std::endl;  // Debug

        // Move, and chain the PerformAttack call as the callback for Move
        Move(positionInRange, [this, animatedTarget, cb = std::move(callback)]() mutable {
            // This lambda is called when Move() finishes (reaches destination or gets blocked)
            sf::Vector2f vec  = animatedTarget->GetPosition() - m_position;
            float        dist = std::sqrt(vec.x * vec.x + vec.y * vec.y);
            if (dist <= m_attackRange * 1.1f)
            {  // Check range again (allow small tolerance)
                std::cout << GetName() << " reached target, performing attack on "
                          << animatedTarget->GetName() << std::endl;  // Debug
                this->PerformAttack(*animatedTarget, std::move(cb));  // Now perform the attack
            }
            else
            {
                std::cout << GetName() << " move finished but target still out of range or blocked."
                          << std::endl;  // Debug
                if (cb)
                    cb();  // Call original callback if attack couldn't proceed
            }
        });
    }
    else
    {
        // Target already in range
        std::cout << GetName() << " is in range, performing attack on " << target.GetName()
                  << std::endl;  // Debug
        PerformAttack(*animatedTarget, std::move(callback));
    }
}

void Assassin::PerformAttack(AnimatedUnit& target, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {
        if (callback)
            callback();
        return;
    }

    // Face the target
    UpdateDirection(target.GetPosition() - m_position);

    // Play attack animation. The lambda executes *after* the animation finishes.
    PlayAnimation(UnitAnimationType::ATTACK, [this, &target, cb = std::move(callback)]() mutable {
        // --- Animation Finished Callback ---
        std::cout << GetName() << "'s attack animation finished." << std::endl;  // Debug

        // Check if target is STILL valid and in range after animation delay
        if (target.IsActive() && target.GetHealth() > 0)
        {
            sf::Vector2f vectorToTarget = target.GetPosition() - m_position;
            float        distanceToTarget =
                std::sqrt(vectorToTarget.x * vectorToTarget.x + vectorToTarget.y * vectorToTarget.y);

            // Deal damage if still in range (maybe slightly larger range check here?)
            if (distanceToTarget <= m_attackRange * 1.1f)  // Allow slight tolerance
            {
                std::cout << GetName() << " deals " << m_attackDamage << " damage to "
                          << target.GetName() << std::endl;  // Debug
                // Cast target back to AnimatedUnit if TakeDamage is needed
                // This is risky if target might not be AnimatedUnit, but necessary for TakeDamage
                AnimatedUnit* animatedTarget = dynamic_cast<AnimatedUnit*>(&target);
                if (animatedTarget)
                {
                    animatedTarget->TakeDamage(m_attackDamage);  // Target takes damage
                }
                else
                {
                    std::cerr << "Warning: Target " << target.GetName()
                              << " is not an AnimatedUnit, cannot TakeDamage." << std::endl;
                }
            }
            else
            {
                std::cout << GetName()
                          << " dealt no damage, target moved out of range during animation."
                          << std::endl;  // Debug
            }
        }
        else
        {
            std::cout << GetName() << " dealt no damage, target is no longer valid."
                      << std::endl;  // Debug
        }

        // Set cooldown AFTER attack attempt
        // m_currentAttackCooldown = m_attackCooldown;

        // Default back to idle if not moving etc.
        if (!m_isMoving)
        {
            PlayAnimation(UnitAnimationType::IDLE);
        }

        // Call the original completion callback if provided
        if (cb)
        {
            cb();
        }
    });
}

void Assassin::UseSkill(int skillId, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return;
    }
}

// Optional: Override RenderUI if Fighter has unique elements
// void Assassin::RenderUI(sf::RenderWindow& window) {
//     Character::RenderUI(window); // Call base UI rendering
//     // Add fighter-specific UI elements here (e.g., rage bar?)
// }