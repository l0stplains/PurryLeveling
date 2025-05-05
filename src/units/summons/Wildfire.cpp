
#include "units/summons/Wildfire.hpp"

#include <cmath>       // For sqrt
#include <functional>  // For std::function
#include <iostream>    // For debug output
#include <limits>

Wildfire::Wildfire(const std::string&  name,
                   const sf::Vector2f& position,
                   NavigationGrid&     navGrid,
                   bool                isPlayerControlled,
                   const GameContext&  gameContext)
    : Unit(name),  // ← must initialize the virtual base
      Summon(name),
      AnimatedUnit(name, position, navGrid, isPlayerControlled, gameContext)
{
    m_moveSpeed = 250.f;
    sf::Vector2i wildfireFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> wildfireTexturePaths = {
        {UnitAnimationType::IDLE, "mob_wildfire_idle"},
        {UnitAnimationType::WALK, "mob_wildfire_walk"},
        {UnitAnimationType::ATTACK, "mob_wildfire_attack"},
        {UnitAnimationType::JUMP, "mob_wildfire_jump"},
        {UnitAnimationType::DAMAGE, "mob_wildfire_dmg"},
        {UnitAnimationType::DIE, "mob_wildfire_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> wildfireShadowTexturePaths = {
        {UnitAnimationType::IDLE, "mob_wildfire_idle_shadow"},
        {UnitAnimationType::WALK, "mob_wildfire_walk_shadow"},
        {UnitAnimationType::ATTACK, "mob_wildfire_attack_shadow"},
        {UnitAnimationType::JUMP, "mob_wildfire_jump_shadow"},
        {UnitAnimationType::DAMAGE, "mob_wildfire_dmg_shadow"},
        {UnitAnimationType::DIE, "mob_wildfire_die_shadow"}

    };
    std::unordered_map<UnitAnimationType, int> wildfireFramesPerAnim = {
        {UnitAnimationType::IDLE, 4},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 12},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> wildfireDurationPerAnim = {
        {UnitAnimationType::IDLE, 0.8f},
        {UnitAnimationType::WALK, 0.6f},
        {UnitAnimationType::ATTACK, 0.6f},
        {UnitAnimationType::JUMP, 0.4f},
        {UnitAnimationType::DAMAGE, 0.4f},
        {UnitAnimationType::DIE, 0.f}};

    std::unordered_map<UnitAnimationType, bool> wildfireLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> wildfireDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> wildfireDefaultRows = {{UnitAnimationType::DIE, 0}};

    LoadAnimations(wildfireTexturePaths,
                   wildfireFrameSize,
                   wildfireFramesPerAnim,
                   wildfireDurationPerAnim,
                   wildfireLoopingAnims,
                   wildfireDirectionalAnims,
                   wildfireDefaultRows,
                   wildfireShadowTexturePaths);
}

void Wildfire::Attack(Unit& target, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {  // Check self and target state
        if (callback)
            callback();  // Cannot attack, call callback immediately
        return;
    }

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
        Move(positionInRange,
             [this,
              animatedTarget,
              initialPosition = m_position,
              initialDir      = m_direction,
              cb              = callback,
              od              = onDeath]() mutable {
                 // This lambda is called when Move() finishes (reaches destination or gets blocked)
                 sf::Vector2f vec  = animatedTarget->GetPosition() - m_position;
                 float        dist = std::sqrt(vec.x * vec.x + vec.y * vec.y);

                 if (dist <= m_attackRange * 1.1f)
                 {  // Check range again (allow small tolerance)
                     std::cout << GetName() << " reached target, performing attack on "
                               << animatedTarget->GetName() << std::endl;  // Debug

                     this->PerformAttack(*animatedTarget, cb, od);
                 }
                 else
                 {
                     std::cout << GetName()
                               << " move finished but target still out of range or blocked."
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
        PerformAttack(*animatedTarget, std::move(callback), std::move(onDeath));
    }
}

void Wildfire::PerformAttack(AnimatedUnit&            target,
                             ActionCompletionCallback callback,
                             ActionCompletionCallback onDeath)
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
    PlayAnimation(
        UnitAnimationType::ATTACK,
        [this, &target, cb = std::move(callback), od = std::move(onDeath)]() mutable {
            // --- Animation Finished Callback ---
            std::cout << GetName() << "'s attack animation finished." << std::endl;  // Debug

            // Check if target is STILL valid and in range after animation delay
            if (target.IsActive() && target.GetHealth() > 0)
            {
                sf::Vector2f vectorToTarget   = target.GetPosition() - m_position;
                float        distanceToTarget = std::sqrt(vectorToTarget.x * vectorToTarget.x +
                                                   vectorToTarget.y * vectorToTarget.y);

                // Deal damage if still in range (maybe slightly larger range check here?)
                if (distanceToTarget <= m_attackRange * 1.1f)  // Allow slight tolerance
                {
                    std::cout << GetName() << " deals " << m_attackDamage << " damage to "
                              << target.GetName() << std::endl;  // Debug
                    // Cast target back to AnimatedUnit if TakeDamage is needed
                    // This is risky if target might not be AnimatedUnit, but necessary
                    // for TakeDamage
                    AnimatedUnit* animatedTarget = dynamic_cast<AnimatedUnit*>(&target);
                    if (animatedTarget)
                    {
                        animatedTarget->TakeDamage(CalculateDamage(target), nullptr, od);  // Target
                                                                                           // takes
                                                                                           // damage
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
                              << " dealt no damage, target moved out of range during "
                                 "animation."
                              << std::endl;  // Debug
                }
            }
            else
            {
                std::cout << GetName() << " dealt no damage, target is no longer valid."
                          << std::endl;  // Debug
            }
            SetActive(false);
            if (cb)
            {
                cb();
            }
        });
}

void Wildfire::UseSkill(Unit&                    target,
                        ActionCompletionCallback callback,
                        ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return;
    }
}

// Optional: Override RenderUI if Fighter has unique elements
// void Wildfire::RenderUI(sf::RenderWindow& window) {
//     Character::RenderUI(window); // Call base UI rendering
//     // Add fighter-specific UI elements here (e.g., rage bar?)
// }