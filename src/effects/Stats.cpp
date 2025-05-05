#include "effects/Stats.hpp"

Stats Stats::operator+(const Stats& other) const
{
    Stats result = *this;
    result += other;
    return result;
}

Stats Stats::operator-(const Stats& other) const
{
    Stats result = *this;
    result -= other;
    return result;
}

Stats& Stats::operator+=(const Stats& other)
{
    // additive stats
    strength += other.strength;
    intelligence += other.intelligence;
    agility += other.agility;
    luck += other.luck;
    physicalDefense += other.physicalDefense;
    magicDefense += other.magicDefense;

    // additive chances/resistances
    criticalStrikeChance += other.criticalStrikeChance;
    skipTurnChance += other.skipTurnChance;
    dodgeChance += other.dodgeChance;
    statusResistance += other.statusResistance;

    // multiplicative stats
    buffMultiplier *= other.buffMultiplier;
    criticalStrikeMultiplier *= other.criticalStrikeMultiplier;
    accuracy *= other.accuracy;
    hasteMultiplier *= other.hasteMultiplier;
    resourceCostMul *= other.resourceCostMul;

    return *this;
}

Stats& Stats::operator-=(const Stats& other)
{
    // subtract additive stats
    strength -= other.strength;
    intelligence -= other.intelligence;
    agility -= other.agility;
    luck -= other.luck;
    physicalDefense -= other.physicalDefense;
    magicDefense -= other.magicDefense;

    // subtract additive chances/resistances
    criticalStrikeChance -= other.criticalStrikeChance;
    skipTurnChance -= other.skipTurnChance;
    dodgeChance -= other.dodgeChance;
    statusResistance -= other.statusResistance;

    // divide multiplicative stats
    if (other.buffMultiplier != 0.0f)
        buffMultiplier /= other.buffMultiplier;
    if (other.criticalStrikeMultiplier != 0.0f)
        criticalStrikeMultiplier /= other.criticalStrikeMultiplier;
    if (other.accuracy != 0.0f)
        accuracy /= other.accuracy;
    if (other.hasteMultiplier != 0.0f)
        hasteMultiplier /= other.hasteMultiplier;
    if (other.resourceCostMul != 0.0f)
        resourceCostMul /= other.resourceCostMul;

    return *this;
}