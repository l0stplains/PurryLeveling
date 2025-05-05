#pragma once
#include <string>

#include "effects/Stats.hpp"

class Effect
{
public:
    Effect(const std::string& name, const std::string& description, int duration);
    virtual ~Effect() = default;

    Effect(const Effect&)            = default;
    Effect& operator=(const Effect&) = default;
    Effect(Effect&&)                 = default;
    Effect& operator=(Effect&&)      = default;

    // --- Getters ---
    const std::string& GetName() const;
    const std::string& GetDescription() const;
    int                GetDuration() const;
    int                GetRemainingDuration() const;
    const Stats&       GetModifiers() const;

    // --- Setters ---
    void SetName(const std::string& name);
    void SetDescription(const std::string& description);
    void SetDuration(int duration);
    void SetRemainingDuration(int remainingDuration);
    void SetModifiers(const Stats& modifiers);

protected:
    std::string m_name;
    std::string m_description;
    int         m_duration;  // in turns
    int         m_remainingDuration;
    Stats       m_modifiers;
};