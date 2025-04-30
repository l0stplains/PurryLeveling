#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <deque>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "core/GameContext.hpp"

#include "Animation.hpp"
#include "Enums.hpp"
#include "NavigationGrid.hpp"
#include "Unit.hpp"

/**
 * @brief Represents a Unit that is visible, animated, and can move.
 * Inherits core stats (health, mana, etc.) from Unit.
 */
class AnimatedUnit : virtual public Unit
{
public:
    AnimatedUnit(const std::string&  name,
                 const sf::Vector2f& position,
                 NavigationGrid&     navGrid,
                 bool                isPlayerControlled,
                 const GameContext&  gameContext);

    ~AnimatedUnit() override = default;

    // Prevent copying, allow moving
    AnimatedUnit(const AnimatedUnit&)            = delete;
    AnimatedUnit& operator=(const AnimatedUnit&) = delete;
    AnimatedUnit(AnimatedUnit&&)                 = default;
    AnimatedUnit& operator=(AnimatedUnit&&)      = default;

    void ProcessEvent(const sf::Event& event);

    /**
     * @brief Updates movement and animation. Regeneration is handled by base Unit if implemented
     * there.
     */
    void Update(const sf::Time& dt);

    /**
     * @brief Draws the unit's sprite.
     */
    void Draw(sf::RenderWindow& window);

    void RenderUI(sf::RenderWindow& window);
    /**
     * @brief Loads textures and creates Animation objects.
     * (Implementation Details as before)
     */
    virtual bool LoadAnimations(
        const std::unordered_map<UnitAnimationType, std::string>& texturePaths,
        const sf::Vector2i&                                       frameSize,
        const std::unordered_map<UnitAnimationType, int>&         framesPerAnim,
        const std::unordered_map<UnitAnimationType, float>&       durationPerAnim,
        const std::unordered_map<UnitAnimationType, bool>&        loopingAnims,
        const std::unordered_map<UnitAnimationType, bool>&        directionalAnims   = {},
        const std::unordered_map<UnitAnimationType, int>&         defaultRows        = {},
        const std::unordered_map<UnitAnimationType, std::string>& shadowTexturePaths = {});

    // --- Core Actions (Overrides base Unit to add animation) ---
    /**
     * @brief Moves the unit towards a target position using the navigation grid.
     */
    virtual void Move(const sf::Vector2f&      targetPosition,
                      ActionCompletionCallback callback = nullptr);

    /**
     * @brief Overrides Unit::TakeDamage to play damage/death animations.
     */
    void TakeDamage(int damage, ActionCompletionCallback callback = nullptr) override;

    /**
     * @brief Overrides Unit::Heal potentially to add a visual effect.
     */
    void Heal(int amount) override;  // Override if visual needed

    /**
     * @brief Resets unit state (health, mana, position, animation).
     */
    virtual void Reset();

    // --- Getters (Stats are inherited from Unit) ---
    const sf::Vector2f& GetPosition() const;
    int                 GetZOrder() const;
    Direction           GetDirection() const;
    const sf::Vector2f& GetVelocity() const;
    const sf::Vector2f& GetScale() const;
    bool                IsMoving() const;

    // --- Setters (Stats setters inherited unless overridden) ---
    void SetZOrder(int zOrder);
    void SetScale(const sf::Vector2f& scale);
    void SetScale(float scaleX, float scaleY);
    void SetMoveSpeed(float speed);

    virtual void SetPosition(const sf::Vector2f& position);
    void         SetDirection(Direction dir);

    void SetControlledByPlayer(bool controlled);
    void SetShowUI(bool show);

    bool IsPlayerControlled() const;

    /**
     * @brief Plays a specific animation type.
     */
    virtual void PlayAnimation(UnitAnimationType        type,
                               ActionCompletionCallback callback   = nullptr,
                               bool                     forceReset = false);

protected:
    /**
     * @brief Updates the unit's direction based on a movement vector.
     */
    void UpdateDirection(const sf::Vector2f& movement);

    /**
     * @brief Gets the sprite sheet row corresponding to the current direction.
     */
    virtual int GetAnimationRowForDirection(Direction dir) const;

    /**
     * @brief Checks if the unit can move to the specified position.
     */
    bool CanMoveTo(const sf::Vector2f& targetPosition) const;

    sf::Vector2f m_position;

    // --- Movement & Position ---
    sf::Vector2f    m_velocity  = {0.f, 0.f};
    float           m_moveSpeed = 100.f;  // Pixels per second
    sf::Vector2f    m_targetPosition;
    bool            m_isMoving = false;
    NavigationGrid& m_navGrid;

    const GameContext& m_gameContext;  ///< Reference to the game context for resource management

    // --- Visuals & Animation ---
    std::optional<sf::Sprite>                          m_sprite;
    std::optional<sf::Sprite>                          m_shadowSprite;  // Optional shadow sprite
    sf::Vector2f                                       m_scale     = {1.0f, 1.0f};
    Direction                                          m_direction = Direction::SOUTH;
    int                                                m_zOrder    = 0;
    std::unordered_map<UnitAnimationType, sf::Texture> m_textures;
    std::unordered_map<UnitAnimationType, sf::Texture> m_shadowTextures;  // Optional shadow
                                                                          // textures
    std::unordered_map<UnitAnimationType, std::unique_ptr<Animation>> m_shadowAnimations;
    std::unordered_map<UnitAnimationType, std::unique_ptr<Animation>> m_animations;
    UnitAnimationType m_currentAnimationType = UnitAnimationType::IDLE;
    Animation*        m_currentAnimation     = nullptr;
    bool              m_isAnimating          = false;

    // --- State & Callbacks ---
    ActionCompletionCallback m_currentActionCallback = nullptr;
    ActionCompletionCallback m_currentMovingCallback = nullptr;

    std::deque<sf::Keyboard::Key> m_moveQueue;

    // --- Control ---
    bool m_controlledByPlayer = false;

    bool m_isHovered = false;  ///< Flag to indicate if the unit is hovered
    bool m_showUI    = false;  ///< Flag to show/hide UI elements
};