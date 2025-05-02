#pragma once

#include <SFML/Window/Keyboard.hpp>  // For input processing

#include <deque>  // For move queue

#include "units/Unit.hpp"

class Character : virtual public Unit
{
public:
    Character(const std::string& name);

    ~Character() override = default;

    // Prevent copying, allow moving
    Character(const Character&)            = delete;
    Character& operator=(const Character&) = delete;
    Character(Character&&)                 = default;
    Character& operator=(Character&&)      = default;

    // --- RPG Stats Getters ---
    int GetLevel() const;
    int GetExp() const;
    int GetGold() const;
    int GetMastery() const;

    // --- RPG Stats Modifiers ---
    void AddExp(int amount);
    void AddGold(int amount);
    void AddMastery(int amount);

protected:
    /**
     * @brief Checks if enough EXP for level up and applies it.
     */
    virtual void CheckLevelUp();

    // --- RPG Stats ---
    int m_level   = 1;
    int m_exp     = 0;
    int m_gold    = 0;
    int m_mastery = 0;
};