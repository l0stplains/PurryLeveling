#include "effects/Effect.hpp"

Effect::Effect(const std::string& name, const std::string& description, int duration)
    : m_name(name), m_description(description), m_duration(duration), m_remainingDuration(duration)
{}

// --- Getters ---
const std::string& Effect::GetName() const
{
    return m_name;
}
const std::string& Effect::GetDescription() const
{
    return m_description;
}
int Effect::GetDuration() const
{
    return m_duration;
}
int Effect::GetRemainingDuration() const
{
    return m_remainingDuration;
}

// --- Setters ---
void Effect::SetName(const std::string& name)
{
    m_name = name;
}
void Effect::SetDescription(const std::string& description)
{
    m_description = description;
}
void Effect::SetDuration(int duration)
{
    m_duration = duration;
}
void Effect::SetRemainingDuration(int remainingDuration)
{
    m_remainingDuration = remainingDuration;
}