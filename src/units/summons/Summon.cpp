#include "units/summons/Summon.hpp"

#include <cmath>
#include <iostream>  // For debug

Summon::Summon(const std::string& name) : Unit(name)  // Call base constructor
{
    // Summon specific stat initialization (can override AnimatedUnit defaults)
    m_maxHealth = 120;  // Example: Summons slightly tougher
    SetHealth(m_maxHealth);
    m_maxMana = 60;
    SetCurrentMana(m_maxMana);
    m_attackDamage = 12;
}

int Summon::GetLevel() const
{
    return m_level;
}

void Summon::SetLevel(int level)
{
    if (!m_active)
        return;
    m_level = level;

    SetMaxHealth(20 * level / 4 * GetMaxHealthMultiplier());
    SetAttackDamage(10 * level / 4 * GetAttackDamageMultiplier());
    SetHealth(GetMaxHealth());
    SetCurrentMana(GetMaxMana());
}

std::vector<sf::Vector2f> Summon::generateSummonSpawnPoints(const sf::Vector2f& center,
                                                            unsigned int        summonCount,
                                                            bool                isRightHalf)
{
    float                     radius = 80.f;
    std::vector<sf::Vector2f> spawnPoints;
    spawnPoints.reserve(summonCount);

    if (summonCount == 1)
    {
        spawnPoints.emplace_back(center);
        return spawnPoints;
    }

    // define start angle based on which half-circle:
    // Right  half: from -90° to +90°  (i.e. -π/2 … +π/2)
    // Left   half: from +90° to +270° (i.e.  π/2 … 3π/2)
    float startAngle = (isRightHalf) ? -M_PI_2 : M_PI_2;
    float arc        = M_PI;  // 180° sweep

    // divide that arc into (mobCount–1) segments so endpoints line up
    float step = arc / static_cast<float>(summonCount - 1);

    for (unsigned int i = 0; i < summonCount; ++i)
    {
        float angle = startAngle + step * static_cast<float>(i);
        float x     = center.x + std::cos(angle) * radius;
        float y     = center.y + std::sin(angle) * radius;
        spawnPoints.emplace_back(x, y);
    }

    return spawnPoints;
}
