#include "units/mobs/basics/Skeleton.hpp"

#include <cmath>     // For sqrt
#include <iostream>  // For debug output
#include <limits>

#include "units/mobs/basics/BasicMob.hpp"

Skeleton::Skeleton(const std::string&  name,
                   const sf::Vector2f& position,
                   NavigationGrid&     navGrid,
                   const GameContext&  gameContext)
    : Unit(name),
      m_boneShard(true, true),
      Mob(name),
      BasicMob(name),  // ← must initialize the virtual base
      AnimatedUnit(name, position, navGrid, false, gameContext)
{
    // Fighter-specific stat overrides
    m_skillProbabability = 0.25f;
    m_moveSpeed          = 200.f;  // Maybe slightly slower, heavier armor?
    m_attackRange        = 48.f;

    sf::Vector2i skeletonFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> skeletonTexturePaths = {
        {UnitAnimationType::IDLE, "mob_skeleton_idle"},
        {UnitAnimationType::WALK, "mob_skeleton_walk"},
        {UnitAnimationType::ATTACK, "mob_skeleton_attack"},
        {UnitAnimationType::JUMP, "mob_skeleton_jump"},
        {UnitAnimationType::DAMAGE, "mob_skeleton_dmg"},
        {UnitAnimationType::DIE, "mob_skeleton_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> skeletonShadowTexturePaths = {
        {UnitAnimationType::IDLE, "mob_skeleton_idle_shadow"},
        {UnitAnimationType::WALK, "mob_skeleton_walk_shadow"},
        {UnitAnimationType::ATTACK, "mob_skeleton_attack_shadow"},
        {UnitAnimationType::JUMP, "mob_skeleton_jump_shadow"},
        {UnitAnimationType::DAMAGE, "mob_skeleton_dmg_shadow"},
        {UnitAnimationType::DIE, "mob_skeleton_die_shadow"}

    };
    // Example frame counts (adjust these!)
    std::unordered_map<UnitAnimationType, int> skeletonFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 9}};

    // Example durations (in seconds, adjust these!)
    std::unordered_map<UnitAnimationType, float> skeletonDurationPerAnim = {
        {UnitAnimationType::IDLE, 3.2f},
        {UnitAnimationType::WALK, 0.8f},
        {UnitAnimationType::ATTACK, 0.4f},
        {UnitAnimationType::JUMP, 0.4f},
        {UnitAnimationType::DAMAGE, 0.4f},
        {UnitAnimationType::DIE, 0.9f}};

    // Example looping status (Idle/Walk usually loop)
    std::unordered_map<UnitAnimationType, bool> skeletonLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> skeletonDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> skeletonDefaultRows = {{UnitAnimationType::DIE, 0}};

    LoadAnimations(skeletonTexturePaths,
                   skeletonFrameSize,
                   skeletonFramesPerAnim,
                   skeletonDurationPerAnim,
                   skeletonLoopingAnims,
                   skeletonDirectionalAnims,
                   skeletonDefaultRows,
                   skeletonShadowTexturePaths);
}

void Skeleton::Attack(Unit& target, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
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
              cb              = std::move(callback),
              od              = std::move(onDeath)]() mutable {
                 // This lambda is called when Move() finishes (reaches destination or gets blocked)
                 sf::Vector2f vec  = animatedTarget->GetPosition() - m_position;
                 float        dist = std::sqrt(vec.x * vec.x + vec.y * vec.y);

                 if (dist <= m_attackRange * 1.1f)
                 {  // Check range again (allow small tolerance)
                     std::cout << GetName() << " reached target, performing attack on "
                               << animatedTarget->GetName() << std::endl;  // Debug

                     // Create a wrapped callback that will handle returning to position
                     // We need to wrap this in a std::shared_ptr to avoid potential lifetime issues
                     auto wrappedCallback = std::make_shared<ActionCompletionCallback>(
                         [this, initialPosition, initialDir, originalCb = std::move(cb)]() mutable {
                             std::cout << GetName()
                                       << " attack completed, returning to initial position"
                                       << std::endl;

                             // Move back to initial position
                             Move(initialPosition,
                                  [this, initialDir, originalCb = std::move(originalCb)]() mutable {
                                      // After moving back, set direction
                                      SetDirection(initialDir);

                                      // Call original callback
                                      if (originalCb)
                                          originalCb();
                                  });
                         });

                     // Call the original PerformAttack with our wrapped callback
                     // The lambda captures a shared_ptr by value to ensure it stays alive
                     this->PerformAttack(
                         *animatedTarget,
                         [wrappedCb = wrappedCallback]() {
                             // Execute our wrapped callback when PerformAttack finishes
                             if (*wrappedCb)
                                 (*wrappedCb)();
                         },
                         od);
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

void Skeleton::PerformAttack(AnimatedUnit&            target,
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
            if (cb)
            {
                cb();
            }
        });
}

bool Skeleton::UseSkill(Unit&                    target,
                        ActionCompletionCallback callback,
                        ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return false;
    }

    PlayAnimation(UnitAnimationType::JUMP, callback);

    return true;
}

// Optional: Override RenderUI if Fighter has unique elements
// void Skeleton::RenderUI(sf::RenderWindow& window) {
//     Character::RenderUI(window); // Call base UI rendering
//     // Add fighter-specific UI elements here (e.g., rage bar?)
// }