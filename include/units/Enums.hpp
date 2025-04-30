#pragma once

/**
 * @brief Enum for unit animations/actions
 */
enum class UnitAnimationType
{
    IDLE,
    WALK,
    JUMP,
    ATTACK,
    CHARGED_ATTACK,
    DAMAGE,
    DIE,
    SKILL
};

/**
 * @brief Enum for unit directions (typically for sprite sheet rows)
 */
enum class Direction
{
    SOUTH,  // Row 0
    WEST,   // Row 1
    EAST,   // Row 2
    NORTH   // Row 3
};