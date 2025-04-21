#include "core/UnitManager.hpp"

UnitManager::UnitManager()
{
    // Constructor
}

UnitManager::~UnitManager()
{
    // The unique_ptr will handle cleanup automatically
}

void UnitManager::AddUnit(std::unique_ptr<Unit> unit)
{
    unsigned int id   = unit->GetId();
    std::string  name = unit->GetName();
    // Store raw pointers for fast lookup
    m_unitsById[id] = unit.get();

    // Only store in name map if name is not empty
    if (!name.empty())
    {
        m_unitsByName[name] = unit.get();
    }

    // Transfer ownership to the manager
    m_units.push_back(std::move(unit));
}

void UnitManager::RemoveUnit(unsigned int id)
{
    auto it = std::find_if(m_units.begin(), m_units.end(), [id](const std::unique_ptr<Unit>& unit) {
        return unit->GetId() == id;
    });
    if (it != m_units.end())
    {
        // Remove from lookup maps first
        std::string name = (*it)->GetName();
        if (!name.empty())
        {
            m_unitsByName.erase(name);
        }

        m_unitsById.erase(id);

        // Remove from main container
        m_units.erase(it);
    }
}

Unit* UnitManager::GetUnit(unsigned int id)
{
    auto it = m_unitsById.find(id);
    return (it != m_unitsById.end()) ? it->second : nullptr;
}

Unit* UnitManager::GetUnitByName(const std::string& name)
{
    auto it = m_unitsByName.find(name);
    return (it != m_unitsByName.end()) ? it->second : nullptr;
}

template <typename T>
T* UnitManager::GetUnitOfType(unsigned int id)
{
    Unit* unit = GetUnit(id);
    return unit ? dynamic_cast<T*>(unit) : nullptr;
}

template <typename T>
std::vector<T*> UnitManager::GetAllUnitsOfType()
{
    std::vector<T*> result;
    for (const auto& unit : m_units)
    {
        if (T* derivedUnit = dynamic_cast<T*>(unit.get()))
        {
            result.push_back(derivedUnit);
        }
    }

    return result;
}
void UnitManager::Update(const sf::Time& dt)
{
    // Update all active units
    for (auto& unit : m_units)
    {
        if (unit->IsActive())
        {
            unit->Update(dt);
        }
    }
    // Remove any units marked for deletion
    // This would be implemented if we had a "marked for deletion" flag
}

void UnitManager::Draw(sf::RenderWindow& window)
{
    // Draw all active units
    for (auto& unit : m_units)
    {
        if (unit->IsActive())
        {
            unit->Draw(window);
        }
    }
}

void UnitManager::Clear()
{
    m_units.clear();
    m_unitsById.clear();
    m_unitsByName.clear();
}

size_t UnitManager::GetUnitCount() const
{
    return m_units.size();
}