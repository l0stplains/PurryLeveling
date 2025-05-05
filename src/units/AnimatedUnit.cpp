#include "units/AnimatedUnit.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>  // For debug messages

#include "units/NavigationGrid.hpp"

AnimatedUnit::AnimatedUnit(const std::string&  name,
                           const sf::Vector2f& position,
                           NavigationGrid&     navGrid,
                           bool                isPlayerControlled,
                           const GameContext&  gameContext)
    : Unit(name),
      m_navGrid(navGrid),
      m_position(position),
      m_targetPosition(position),
      m_controlledByPlayer(isPlayerControlled),
      m_gameContext(gameContext)
{}

// --- Update & Draw ---
void AnimatedUnit::ProcessEvent(const sf::Event& event)
{
    if (auto k = event.getIf<sf::Event::KeyPressed>())
    {
        if (k->code == sf::Keyboard::Key::W || k->code == sf::Keyboard::Key::A ||
            k->code == sf::Keyboard::Key::S || k->code == sf::Keyboard::Key::D)
        {
            // remove any old instance
            m_moveQueue.erase(std::remove(m_moveQueue.begin(), m_moveQueue.end(), k->code),
                              m_moveQueue.end());
            m_moveQueue.push_back(k->code);
        }
    }
    else if (auto k = event.getIf<sf::Event::KeyReleased>())
    {
        if (k->code == sf::Keyboard::Key::W || k->code == sf::Keyboard::Key::A ||
            k->code == sf::Keyboard::Key::S || k->code == sf::Keyboard::Key::D)
        {
            m_moveQueue.erase(std::remove(m_moveQueue.begin(), m_moveQueue.end(), k->code),
                              m_moveQueue.end());
        }
    }
}
void AnimatedUnit::RenderUI(sf::RenderWindow& window)
{
    if (!m_active || !m_showUI)
        return;

    if (m_isHovered)
    {
        float barWidth  = 50.0f;
        float barHeight = 5.0f;

        sf::Vector2f healthBarPosition = m_position + sf::Vector2f(-barWidth / 2.0f, -64.0f);
        sf::Vector2f manaBarPosition   = m_position + sf::Vector2f(-barWidth / 2.0f, -54.0f);
        sf::Vector2f namePosition      = m_position + sf::Vector2f(0, 36.0f);
        sf::Vector2f levelPosition     = m_position + sf::Vector2f(-barWidth / 2.0f, 28.0f);

        // Background (Health)
        sf::RectangleShape healthBarBg(sf::Vector2f(barWidth, barHeight));
        healthBarBg.setFillColor(sf::Color(50, 50, 50));  // Dark grey
        healthBarBg.setPosition(healthBarPosition);

        // Foreground (Health)
        float healthPercent = static_cast<float>(m_currentHealth) / static_cast<float>(m_maxHealth);
        healthPercent       = std::max(0.0f, std::min(1.0f, healthPercent));  // Clamp 0-1
        sf::RectangleShape healthBarFg(sf::Vector2f(barWidth * healthPercent, barHeight));
        healthBarFg.setFillColor(sf::Color::Red);
        healthBarFg.setPosition(healthBarPosition);

        // Background (Mana)
        sf::RectangleShape manaBarBg(sf::Vector2f(barWidth, barHeight));
        manaBarBg.setFillColor(sf::Color(50, 50, 50));  // Dark grey
        manaBarBg.setPosition(manaBarPosition);

        // Foreground (mana)
        float manaPercent = static_cast<float>(m_currentMana) / static_cast<float>(m_maxMana);
        manaPercent       = std::max(0.0f, std::min(1.0f, manaPercent));  // Clamp 0-1
        sf::RectangleShape manaBarFg(sf::Vector2f(barWidth * manaPercent, barHeight));
        manaBarFg.setFillColor(sf::Color::Blue);
        manaBarFg.setPosition(manaBarPosition);

        sf::Text name(m_gameContext.GetResourceManager()->GetFont("main_font"));
        name.setString(m_name);
        name.setCharacterSize(24);
        name.setOutlineColor(sf::Color::Black);
        name.setOutlineThickness(1);
        name.setFillColor(sf::Color::White);

        // center the text
        sf::FloatRect textBounds = name.getLocalBounds();
        name.setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                        textBounds.position.y + textBounds.position.y / 2.f});
        name.setPosition(namePosition);

        window.draw(healthBarBg);
        window.draw(healthBarFg);
        window.draw(manaBarBg);
        window.draw(manaBarFg);
        window.draw(name);
    }

    // Add Mana bar similarly if needed
}

void AnimatedUnit::Update(const sf::Time& dt)
{
    if (!m_active)
        return;

    float deltaTime = dt.asSeconds();

    // -1. Update Floating Texts
    for (auto it = m_floatingTexts.begin(); it != m_floatingTexts.end();)
    {
        if (!it->Update(deltaTime))
        {
            it = m_floatingTexts.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 0. clear queue if suddenly becomes not controllable
    if (!m_controlledByPlayer && m_moveQueue.size())
    {
        // clear queue
        m_moveQueue.clear();
    }

    // 1. Handle Movement
    if (m_isMoving)
    {
        sf::Vector2f currentPosition = m_position;  // Use the Unit's position
        sf::Vector2f vectorToTarget  = m_targetPosition - currentPosition;
        float        distanceToTarget =
            std::sqrt(vectorToTarget.x * vectorToTarget.x + vectorToTarget.y * vectorToTarget.y);

        // Check if close enough to destination
        const float arrivalThreshold = 4.0f;  // Pixels tolerance
        if (distanceToTarget <= arrivalThreshold)
        {
            SetPosition(m_targetPosition);  // Snap to target
            m_velocity = {0.f, 0.f};
            m_isMoving = false;

            // Finished moving, switch to IDLE if current anim was WALK
            if (m_currentAnimationType == UnitAnimationType::WALK)
            {
                PlayAnimation(UnitAnimationType::IDLE);  // Play idle when stopped
            }

            // Execute callback if movement finished
            if (m_currentMovingCallback)
            {
                auto callback = std::move(m_currentMovingCallback);  // Move callback out before
                                                                     // executing
                m_currentMovingCallback = nullptr;                   // Clear it
                callback();
            }
        }
        else
        {
            // Normalize direction vector
            sf::Vector2f direction = vectorToTarget / distanceToTarget;
            m_velocity             = direction * m_moveSpeed;

            // Calculate potential next position
            sf::Vector2f potentialNextPos = currentPosition + m_velocity * deltaTime;

            // Check if the potential next position is walkable using NavigationGrid
            if (m_navGrid.IsWalkable(potentialNextPos))  // Check grid + window bounds
            {
                SetPosition(potentialNextPos);  // Update position if walkable
                UpdateDirection(m_velocity);    // Update facing direction
            }
            else
            {
                // For simplicity, just stop moving for now or try alternative paths.
                m_velocity = {0.f, 0.f};
                m_isMoving = false;  // Stop if blocked
                if (m_currentAnimationType == UnitAnimationType::WALK)
                {
                    PlayAnimation(UnitAnimationType::IDLE);
                }
                // Optionally call callback here if stopping constitutes completion?
                if (m_currentMovingCallback)
                {
                    // std::cout << GetName() << " movement blocked." << std::endl; // Debug
                    auto callback           = std::move(m_currentMovingCallback);
                    m_currentMovingCallback = nullptr;
                    callback();  // Indicate movement ended (blocked)
                }
            }
        }
    }
    else
    {
        m_velocity = {0.f, 0.f};  // Ensure velocity is zero when not actively moving
    }

    // 2. Update Animation
    if (m_currentAnimation && m_isAnimating)
    {
        // Update direction row before updating the frame
        if (m_animations.count(m_currentAnimationType) && m_currentAnimation->IsDirectional())
        {
            m_currentAnimation->SetRow(GetAnimationRowForDirection(m_direction));
        }

        bool justFinished = m_currentAnimation->Update(dt);

        // Apply frame to sprite *after* updating
        m_currentAnimation->ApplyToSprite(*m_sprite);

        if (justFinished && !m_currentAnimation->IsFinished())
        {
            // This case should ideally not happen with the current Animation::Update logic
            // but good for sanity check. It means Update returned true but IsFinished is false.
        }

        // If animation finished (and wasn't looping)
        if (m_currentAnimation->IsFinished())  // Check IsFinished state after Update
        {
            m_isAnimating = false;  // Stop animating (state-wise)

            // Execute callback associated with this specific animation finishing
            if (m_currentActionCallback)
            {
                // std::cout << GetName() << " animation finished, executing callback." <<
                // std::endl; // Debug
                auto callback           = std::move(m_currentActionCallback);  // Move callback out
                m_currentActionCallback = nullptr;  // Clear it before calling
                callback();
                // If the callback started a *new* animation, m_currentActionCallback might be set
                // again
            }

            // Default to IDLE animation if nothing else is playing
            if (!m_isMoving && m_currentAnimationType != UnitAnimationType::IDLE &&
                m_currentAnimationType != UnitAnimationType::DIE)
            {
                PlayAnimation(UnitAnimationType::IDLE);
            }
        }
    }
    else if (m_animations.count(UnitAnimationType::IDLE) &&
             m_currentAnimationType != UnitAnimationType::IDLE)
    {
        // If no animation is playing, default to idle (if available)
        PlayAnimation(UnitAnimationType::IDLE);
        if (m_currentAnimation)
        {  // Ensure idle animation exists and was set
            m_currentAnimation->ApplyToSprite(*m_sprite);
        }
    }
    else if (m_currentAnimation)
    {
        // Apply current (possibly static) frame even if not animating
        m_currentAnimation->ApplyToSprite(*m_sprite);
    }

    // advance shadow too (only if optional has value, and a shadow animation exists)
    if (m_shadowSprite.has_value())
    {
        auto it = m_shadowAnimations.find(m_currentAnimationType);
        if (it != m_shadowAnimations.end())
        {
            it->second->Update(dt);
            it->second->ApplyToSprite(*m_shadowSprite);
        }
    }

    // Character-specific updates after base update
    if (m_active)
    {
        // Potentially check level up periodically or after certain events?
        // CheckLevelUp(); // Might be better to call this only when EXP is gained
        if (m_controlledByPlayer)
        {
            // how far to move per-tile
            const float  step   = m_navGrid.GetTileWidth() * 0.5f;
            sf::Vector2f target = GetPosition();

            if (!m_moveQueue.empty())
            {
                switch (m_moveQueue.back())
                {
                    case sf::Keyboard::Key::W:
                        target.y -= step;
                        break;
                    case sf::Keyboard::Key::S:
                        target.y += step;
                        break;
                    case sf::Keyboard::Key::A:
                        target.x -= step;
                        break;
                    case sf::Keyboard::Key::D:
                        target.x += step;
                        break;
                    default:
                        break;
                }
                Move(target, []() { /* on-finish callback */ });
            }
        }
        sf::Vector2f mouseWorld =
            static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_gameContext.GetWindow()));

        sf::Vector2f mouseLocal = m_sprite->getInverseTransform().transformPoint(mouseWorld);

        sf::FloatRect localBounds = m_sprite->getLocalBounds();

        // Compute the inset (25% on each side → central 50%)
        sf::Vector2f inset {localBounds.size.x * 0.15f, localBounds.size.y * 0.15f};

        // Build the “central” rectangle
        sf::Vector2f  centerPos  = localBounds.position + inset;
        sf::Vector2f  centerSize = localBounds.size * 0.5f;
        sf::FloatRect centralRect(centerPos, centerSize);

        m_isHovered = centralRect.contains(mouseLocal);
    }
}

void AnimatedUnit::Draw(sf::RenderWindow& window)
{
    if (!m_active || !m_sprite.has_value())
        return;

    // sync positions & scale
    m_sprite->setPosition(m_position);
    m_sprite->setScale(m_scale);

    if (m_shadowSprite.has_value())
    {
        // only draw if we loaded a shadow for this animation type
        if (m_shadowAnimations.count(m_currentAnimationType))
        {
            m_shadowSprite->setPosition(m_position);
            m_shadowSprite->setScale(m_scale);
            window.draw(*m_shadowSprite);
        }
    }

    const sf::Font& font = m_gameContext.GetResourceManager()->GetFont("main_font");
    for (auto& floatingText : m_floatingTexts)
    {
        floatingText.Draw(window, font);
    }

    // then the real sprite
    window.draw(*m_sprite);
}

// --- Animation Loading & Playing ---

bool AnimatedUnit::LoadAnimations(
    const std::unordered_map<UnitAnimationType, std::string>& textureId,
    const sf::Vector2i&                                       frameSize,
    const std::unordered_map<UnitAnimationType, int>&         framesPerAnim,
    const std::unordered_map<UnitAnimationType, float>&       durationPerAnim,
    const std::unordered_map<UnitAnimationType, bool>&        loopingAnims,
    const std::unordered_map<UnitAnimationType, bool>&        directionalAnims,
    const std::unordered_map<UnitAnimationType, int>&         defaultRows,
    const std::unordered_map<UnitAnimationType, std::string>& shadowTextureId)
{
    m_animations.clear();
    m_shadowAnimations.clear();
    m_textures.clear();  // Clear existing textures if any
    m_shadowTextures.clear();
    m_currentAnimation = nullptr;
    bool success       = true;

    for (auto& [type, id] : textureId)
    {
        // Load Texture
        sf::Texture texture = m_gameContext.GetResourceManager()->GetTexture(id);

        m_textures[type] = std::move(texture);  // Move texture into the map

        // Get Animation parameters
        int   numFrames     = framesPerAnim.count(type) ? framesPerAnim.at(type) : 1;
        float duration      = durationPerAnim.count(type) ? durationPerAnim.at(type) : 1.0f;
        bool  isLooping     = loopingAnims.count(type)
                                  ? loopingAnims.at(type)
                                  : (type == UnitAnimationType::IDLE ||
                                type == UnitAnimationType::WALK);  // Default looping for idle/walk
        bool  isDirectional = directionalAnims.count(type) ? directionalAnims.at(type) : false;
        int   defaultRow    = defaultRows.count(type) ? defaultRows.at(type) : 0;

        m_animations[type] = std::make_unique<Animation>(
            m_textures.at(type), frameSize, numFrames, duration, isLooping, isDirectional, defaultRow);
    }

    for (auto& [type, id] : shadowTextureId)
    {
        sf::Texture shadowTexture = m_gameContext.GetResourceManager()->GetTexture(id);

        // safe fallbacks exactly like main loop:
        int   numFrames   = framesPerAnim.count(type) ? framesPerAnim.at(type) : 1;
        float duration    = durationPerAnim.count(type) ? durationPerAnim.at(type) : 1.0f;
        bool  looping     = loopingAnims.count(type)
                                ? loopingAnims.at(type)
                                : (type == UnitAnimationType::IDLE || type == UnitAnimationType::WALK);
        bool  directional = directionalAnims.count(type) ? directionalAnims.at(type) : false;
        int   defRow      = defaultRows.count(type) ? defaultRows.at(type) : 0;

        m_shadowTextures[type]   = std::move(shadowTexture);
        m_shadowAnimations[type] = std::make_unique<Animation>(
            m_shadowTextures[type], frameSize, numFrames, duration, looping, directional, defRow);
    }

    m_sprite.emplace(m_textures[UnitAnimationType::IDLE]);
    m_sprite->setPosition(m_position);  // Set initial position

    if (m_sprite)
    {
        m_sprite->setOrigin({frameSize.x / 2.0f, frameSize.y / 2.0f});  // Center origin
    }

    // Default to IDLE animation if available
    if (m_animations.count(UnitAnimationType::IDLE))
    {
        PlayAnimation(UnitAnimationType::IDLE);
    }
    else if (!m_animations.empty())
    {
        // Fallback to the first loaded animation if IDLE is missing
        PlayAnimation(m_animations.begin()->first);
    }
    else
    {
        std::cerr << "Warning: No animations loaded for unit " << m_name << std::endl;
        success = false;  // Indicate failure if no animations loaded
    }

    if (m_shadowTextures.count(UnitAnimationType::IDLE))
    {
        m_shadowSprite.emplace(m_shadowTextures.at(UnitAnimationType::IDLE));
        m_shadowSprite->setOrigin({frameSize.x / 2.f, frameSize.y / 2.f});
    }

    return success;
}

void AnimatedUnit::PlayAnimation(UnitAnimationType        type,
                                 ActionCompletionCallback callback,
                                 bool                     forceReset)
{
    // 0) Fallback if the animation isn’t loaded
    if (m_animations.find(type) == m_animations.end())
    {
        if (type != UnitAnimationType::IDLE && m_animations.count(UnitAnimationType::IDLE))
        {
            type = UnitAnimationType::IDLE;
        }
        else
        {
            m_currentAnimation = nullptr;
            m_isAnimating      = false;
            return;
        }
    }

    // 1) Early‐exit if we’re already playing this animation
    if (m_currentAnimationType == type && m_isAnimating && !forceReset)
    {
        if (callback)
            m_currentActionCallback = std::move(callback);
        return;
    }

    // 2) Switch to the new animation
    m_currentAnimationType = type;
    m_currentAnimation     = m_animations[type].get();
    m_currentAnimation->Reset();

    // 3) Also reset the shadow animation if it exists
    auto sIt = m_shadowAnimations.find(type);
    if (sIt != m_shadowAnimations.end())
    {
        sIt->second->Reset();
    }

    // 4) Compute which row to use
    int row = m_currentAnimation->IsDirectional() ? GetAnimationRowForDirection(m_direction)
                                                  : m_currentAnimation->GetDefaultRow();

    // 5) Apply that row to both main + shadow
    m_currentAnimation->SetRow(row);
    if (sIt != m_shadowAnimations.end())
    {
        sIt->second->SetRow(row);
    }

    // 6) Store the finish‐callback and mark animating
    m_currentActionCallback = std::move(callback);
    m_isAnimating           = true;

    // 7) Apply the very first frame immediately
    m_currentAnimation->ApplyToSprite(*m_sprite);

    if (m_shadowSprite.has_value() && sIt != m_shadowAnimations.end())
    {
        sIt->second->ApplyToSprite(*m_shadowSprite);
    }
}

void AnimatedUnit::AddFloatingText(const std::string& text, const sf::Color& color)
{
    if (!m_active || !m_showUI)
        return;

    // Position it slightly above the unit
    sf::Vector2f textPos = m_position + sf::Vector2f(0, -64.0f);

    // Create and add the floating text
    m_floatingTexts.emplace_back(text, textPos, color);
}

// --- Actions ---

void AnimatedUnit::Move(const sf::Vector2f& targetPosition, ActionCompletionCallback callback)
{
    // 1) Early-out if already at target
    sf::Vector2f delta     = targetPosition - m_position;
    float        distance  = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    const float  threshold = 4.0f;
    if (distance < threshold)
    {
        if (callback)
            callback();
        return;
    }

    // 2) If direct position blocked, scan in current facing direction
    if (!m_navGrid.IsWalkable(targetPosition))
    {
        // Convert target world->grid
        sf::Vector2u grid = m_navGrid.WorldToGridCoords(targetPosition);
        // Determine cardinal delta from m_direction
        sf::Vector2i stepOffset(0, 0);
        switch (m_direction)
        {
            case Direction::NORTH:
                stepOffset = {0, -1};
                break;
            case Direction::SOUTH:
                stepOffset = {0, 1};
                break;
            case Direction::WEST:
                stepOffset = {-1, 0};
                break;
            case Direction::EAST:
                stepOffset = {1, 0};
                break;
        }
        // Walk along the ray until we hit a walkable tile or leave grid
        unsigned int maxSteps = std::max(m_navGrid.GetGridWidth(), m_navGrid.GetGridHeight());
        for (unsigned int i = 1; i <= maxSteps; ++i)
        {
            int gx = static_cast<int>(grid.x) + stepOffset.x * static_cast<int>(i);
            int gy = static_cast<int>(grid.y) + stepOffset.y * static_cast<int>(i);
            if (gx < 0 || gy < 0 || gx >= static_cast<int>(m_navGrid.GetGridWidth()) ||
                gy >= static_cast<int>(m_navGrid.GetGridHeight()))
            {
                break;  // out of bounds
            }
            sf::Vector2u candidate {static_cast<unsigned int>(gx), static_cast<unsigned int>(gy)};
            sf::Vector2f worldCandidate = m_navGrid.GridToWorldCoords(candidate);
            if (m_navGrid.IsWalkable(worldCandidate))
            {
                // Found the nearest along the line
                m_targetPosition        = worldCandidate;
                m_isMoving              = true;
                m_currentMovingCallback = std::move(callback);
                UpdateDirection(m_targetPosition - m_position);
                PlayAnimation(UnitAnimationType::WALK, nullptr);
                return;
            }
        }
        return;
    }

    // 3) Direct move if clear
    m_targetPosition        = targetPosition;
    m_isMoving              = true;
    m_currentMovingCallback = std::move(callback);
    UpdateDirection(delta);
    PlayAnimation(UnitAnimationType::WALK, nullptr);
}

// --- TakeDamage Override ---
void AnimatedUnit::TakeDamage(int                      damage,
                              ActionCompletionCallback callback,
                              ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return;
    }

    RNG rng;

    float dodgeChance = rng.generateProbability();
    if (dodgeChance < m_stats.dodgeChance && damage != 0)
    {
        AddFloatingText("Dodged", sf::Color::White);  // Green dodge text
        damage = 0;                                   // dodged damage
    }

    float missChance = rng.generateProbability();
    if (missChance > m_stats.accuracy && damage != 0)
    {
        AddFloatingText("Missed", sf::Color::White);  // Green miss text
        damage = 0;                                   // missed damage
    }

    // Store current health before taking damage to check if it resulted in death
    int healthBefore = m_currentHealth;

    // Call base class function FIRST to update health and active status
    Unit::TakeDamage(damage, nullptr);  // Don't pass callback here, we handle it after animation

    if (damage == 0)
    {
        // No damage taken, just run the callback
        if (callback)
            callback();
        return;
    }

    std::string damageText = "-" + std::to_string(damage);
    AddFloatingText(damageText, sf::Color(255, 40, 40));  // Bright red

    // Check if the unit just became inactive (died)
    bool justDied = (healthBefore > 0 && m_currentHealth <= 0);

    ActionCompletionCallback finalCallback = std::move(callback);  // Hold the original callback

    if (justDied)
    {
        m_isMoving = false;  // Stop moving if dead
        m_velocity = {0.f, 0.f};
        // Play death animation. Chain the original callback after death anim.
        std::cout << GetName() << " (Animated) just died." << std::endl;  // Debug
        if (onDeath)
        {
            // Call the onDeath callback immediately
            onDeath();
        }
        PlayAnimation(
            UnitAnimationType::DIE,
            [this, cb = std::move(finalCallback)]() {
                std::cout << GetName() << " (Animated) has died." << std::endl;  // Debug
                m_isAnimating = false;
                m_active      = false;  // Set inactive
                if (cb)
                    cb();
            },
            true);
    }
    else if (m_active)  // Still alive, play damage animation
    {
        // Play damage animation. Chain the original callback after damage anim.
        PlayAnimation(
            UnitAnimationType::DAMAGE,
            [this, cb = std::move(finalCallback)]() {
                std::cout << GetName() << " (Animated) took damage." << std::endl;  // Debug
                if (!m_isMoving)
                {
                    PlayAnimation(UnitAnimationType::IDLE);
                }
                if (cb)
                    cb();
            },
            true);  // Force reset damage animation even if already playing
    }
    else
    {
        // Became inactive but wasn't captured by justDied? (Shouldn't happen)
        // Or was already inactive? Run callback immediately.
        if (finalCallback)
            finalCallback();
    }
}

// --- Heal Override (Optional: Add visual effect) ---
void AnimatedUnit::Heal(int amount, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0)
        return;

    Unit::Heal(amount);  // Call base class to update health value
    std::string healText = "+" + std::to_string(amount);
    AddFloatingText(healText, sf::Color(40, 230, 40));  // Bright green

    PlayAnimation(
        UnitAnimationType::JUMP,  // Optional: Play heal animation
        [this, cb = std::move(callback), amount]() {
            std::cout << GetName() << " (Animated) healed for " << amount << "." << std::endl;
            if (cb)
                cb();
        },
        true);  // Force reset heal animation even if already playing
}

void AnimatedUnit::RestoreMana(int amount)
{
    if (!m_active || m_currentHealth <= 0)
        return;

    Unit::RestoreMana(amount);
    std::string manaText = "+" + std::to_string(amount);
    AddFloatingText(manaText, sf::Color::Blue);
}

// --- Reset Override ---
void AnimatedUnit::Reset()
{
    m_currentHealth = m_maxHealth;
    m_currentMana   = m_maxMana;
    m_active        = true;  // Ensure active is true

    // Reset AnimatedUnit specific state
    m_isMoving       = false;
    m_velocity       = {0.f, 0.f};
    m_targetPosition = m_position;
    m_activeEffects.clear();

    PlayAnimation(UnitAnimationType::IDLE);  // Reset animation
}

// --- Setters ---

void AnimatedUnit::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_sprite->setPosition(m_position);  // Update sprite position
    // If moving, we might want to reconsider the target if position is set externally
    // m_targetPosition = m_position; // Option: stop current move if pos is set manually?
    // m_isMoving = false;
}

void AnimatedUnit::SetZOrder(int zOrder)
{
    m_zOrder = zOrder;
}
void AnimatedUnit::SetScale(const sf::Vector2f& scale)
{
    m_scale = scale;
}
void AnimatedUnit::SetScale(float scaleX, float scaleY)
{
    m_scale = {scaleX, scaleY};
}

void AnimatedUnit::SetNavGrid(NavigationGrid& navGrid)
{
    m_navGrid = navGrid;
}

void AnimatedUnit::SetMoveSpeed(float speed)
{
    m_moveSpeed = std::max(0.0f, speed);
}

const sf::Vector2f& AnimatedUnit::GetPosition() const
{
    return m_position;
}

int AnimatedUnit::GetZOrder() const
{
    return m_zOrder;
}
Direction AnimatedUnit::GetDirection() const
{
    return m_direction;
}
const sf::Vector2f& AnimatedUnit::GetVelocity() const
{
    return m_velocity;
}

float AnimatedUnit::GetMoveSpeed() const
{
    return m_moveSpeed;
}

const sf::Vector2f& AnimatedUnit::GetScale() const
{
    return m_scale;
}

const NavigationGrid& AnimatedUnit::GetNavGrid() const
{
    return m_navGrid;
}

const sf::Texture& AnimatedUnit::GetTextures(UnitAnimationType animationType) const
{
    auto it = m_textures.find(animationType);
    if (it != m_textures.end())
    {
        return it->second;
    }
    throw std::runtime_error("Texture not found for animation type");
}

bool AnimatedUnit::IsMoving() const
{
    return m_isMoving;
}

// --- Protected Helpers ---

bool AnimatedUnit::CanMoveTo(const sf::Vector2f& targetPosition) const
{
    return m_navGrid.IsWalkable(targetPosition);
    // This already checks bounds and the grid status
}

void AnimatedUnit::UpdateDirection(const sf::Vector2f& movement)
{
    if (movement.x == 0.f && movement.y == 0.f)
    {
        return;  // No change if no movement
    }

    // Simplified 4-direction logic
    // Prioritize horizontal or vertical based on larger magnitude?
    if (std::abs(movement.x) > std::abs(movement.y))
    {
        // More horizontal movement
        m_direction = (movement.x > 0) ? Direction::EAST : Direction::WEST;
    }
    else
    {
        // More vertical movement (or equal)
        m_direction = (movement.y > 0) ? Direction::SOUTH : Direction::NORTH;
    }
}

int AnimatedUnit::GetAnimationRowForDirection(Direction dir) const
{
    switch (dir)
    {
        case Direction::SOUTH:
            return 0;
        case Direction::WEST:
            return 1;
        case Direction::EAST:
            return 0;
        case Direction::NORTH:
            return 2;
        default:
            return 0;  // Default to first row
    }
}

bool AnimatedUnit::IsPlayerControlled() const
{
    return m_controlledByPlayer;
}

void AnimatedUnit::SetDirection(Direction dir)
{
    m_direction = dir;
    if (m_currentAnimation)
    {
        m_currentAnimation->SetRow(GetAnimationRowForDirection(m_direction));
    }
}

void AnimatedUnit::SetControlledByPlayer(bool controlled)
{
    m_controlledByPlayer = controlled;
    m_moveQueue.clear();
    m_isMoving              = false;
    m_velocity              = {0.f, 0.f};
    m_targetPosition        = m_position;  // don’t march to a stale target
    m_currentMovingCallback = nullptr;     // drop the callback

    PlayAnimation(UnitAnimationType::IDLE);
}

void AnimatedUnit::SetShowUI(bool show)
{
    m_showUI = show;
}