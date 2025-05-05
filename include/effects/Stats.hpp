#pragma once

/// All the numbers that buffs/debuffs should touch.
/// Leave HP/Mana/etc. on your Unit class.
class Stats
{
public:
    Stats()                              = default;
    Stats(const Stats& other)            = default;
    Stats& operator=(const Stats& other) = default;
    Stats(Stats&& other)                 = default;
    Stats& operator=(Stats&& other)      = default;
    ~Stats()                             = default;

    // Operator overloads
    Stats operator+(const Stats& other) const;
    Stats operator-(const Stats& other) const;

    Stats& operator+=(const Stats& other);
    Stats& operator-=(const Stats& other);

    // — Primary attributes —
    int strength     = 0;
    int intelligence = 0;
    int agility      = 0;

    // — Multipliers & chances —
    float buffMultiplier           = 1.0f;  // scales positive buffs
    float criticalStrikeMultiplier = 1.0f;  // damage × this on crit
    float criticalStrikeChance     = 0.0f;  // 0.0–1.0
    float skipTurnChance           = 0.0f;  // 0.0–1.0

    // — Extra stats for more depth —
    int   luck             = 0;     // increases crit and dodge chance
    int   physicalDefense  = 0;     // flat damage reduction
    int   magicDefense     = 0;     // flat magic reduction
    float dodgeChance      = 0.0f;  // chance to avoid an attack
    float accuracy         = 1.0f;  // multiplier on hit‐chance
    float statusResistance = 0.0f;  // shrinks negative effect durations
    float hasteMultiplier  = 1.0f;  // extra actions / faster cooldowns (speed)
    float resourceCostMul  = 1.0f;  // reduces/increases mana/spell costs
};
