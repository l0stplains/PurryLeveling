#include "core/UnitManager.hpp"

#include <iostream>  // For potential debug messages

#include "units/AnimatedUnit.hpp"  // Include derived headers needed for casting
#include "units/Unit.hpp"
#include "units/characters/Character.hpp"

// Constructor/Destructor are defaulted in header now

void UnitManager::AddUnit(std::unique_ptr<Unit> unit)
{
    if (!unit)
        return;
    if (m_inUpdate)
    {
        m_pendingAdditions.push_back(std::move(unit));
    }
    else
    {
        AddUnitImmediate(std::move(unit));
    }
}

// --- Replace your RemoveUnit with this ---
void UnitManager::RemoveUnit(unsigned int id)
{
    if (m_inUpdate)
    {
        m_pendingRemovals.push_back(id);
    }
    else
    {
        RemoveUnitImmediate(id);
    }
}

void UnitManager::RemoveUnitExcept(unsigned int id)
{
    // Remove all units except the one with the given ID
    auto it = std::remove_if(m_units.begin(), m_units.end(), [id](const std::unique_ptr<Unit>& u) {
        return u->GetId() != id;
    });

    // Erase the removed units from the vector
    m_units.erase(it, m_units.end());

    // Clear lookup maps
    m_unitsById.clear();
    m_unitsByName.clear();

    // Rebuild lookup maps for remaining units
    for (const auto& unitPtr : m_units)
    {
        unsigned int id     = unitPtr->GetId();
        std::string  name   = unitPtr->GetName();
        Unit*        rawPtr = unitPtr.get();

        m_unitsById[id] = rawPtr;
        if (!name.empty())
        {
            m_unitsByName[name] = rawPtr;
        }
    }
}

Unit* UnitManager::GetUnit(unsigned int id)
{
    auto it = m_unitsById.find(id);
    // Check if found and if the pointer is still valid (although map should be synced)
    return (it != m_unitsById.end()) ? it->second : nullptr;
}

Unit* UnitManager::GetUnitByName(const std::string& name)
{
    if (name.empty())
        return nullptr;
    auto it = m_unitsByName.find(name);
    return (it != m_unitsByName.end()) ? it->second : nullptr;
}

void UnitManager::Update(const sf::Time& dt)
{
    // 1) Entering update loop
    m_inUpdate = true;

    // 2) Iterate all units
    for (auto& unitPtr : m_units)
    {
        if (unitPtr && unitPtr->IsActive())
        {
            if (auto* animated = dynamic_cast<AnimatedUnit*>(unitPtr.get()))
                animated->Update(dt);
            else
                std::cerr << "Warning: Unit " << unitPtr->GetName()
                          << " is not an AnimatedUnit, cannot Update." << std::endl;
        }
    }

    // 3) Done iterating
    m_inUpdate = false;

    // 4) Flush removals first
    for (auto id : m_pendingRemovals)
        RemoveUnitImmediate(id);
    m_pendingRemovals.clear();

    // 5) Then flush additions
    for (auto& u : m_pendingAdditions)
        AddUnitImmediate(std::move(u));
    m_pendingAdditions.clear();
}

void UnitManager::Draw(sf::RenderWindow& window)
{
    // Ensure units are sorted by Z-order before drawing
    if (m_needsSorting)
    {
        SortUnitsByZOrder();
        m_needsSorting = false;
    }

    // Units vector is now sorted by Z-order (ascending)
    for (const auto& unitPtr : m_units)
    {
        if (unitPtr && unitPtr->IsActive())
        {
            // dynamic cast to animated unit
            if (AnimatedUnit* animatedUnit = dynamic_cast<AnimatedUnit*>(unitPtr.get()))
            {
                animatedUnit->Draw(window);
            }
            else
            {
                std::cerr << "Warning: Unit " << unitPtr->GetName()
                          << " is not an AnimatedUnit, cannot Update." << std::endl;
            }
        }
    }
}

void UnitManager::DrawUI(sf::RenderWindow& window)
{
    // Draw UI elements for relevant units (e.g., Characters)
    for (const auto& unitPtr : m_units)
    {
        if (unitPtr && unitPtr->IsActive())
        {
            if (AnimatedUnit* animatedUnit = dynamic_cast<AnimatedUnit*>(unitPtr.get()))
            {
                animatedUnit->RenderUI(window);
            }
            // Add else if for other types with UI if needed
        }
    }
}

void UnitManager::ProcessEvent(const sf::Event& event)
{
    // Forward keyboard input only to active, player-controlled Characters
    for (auto& unitPtr : m_units)
    {
        if (unitPtr && unitPtr->IsActive())
        {
            if (AnimatedUnit* animatedUnit = dynamic_cast<AnimatedUnit*>(unitPtr.get()))
            {
                // Check if this character is player-controlled
                if (animatedUnit->IsPlayerControlled())
                {
                    // Call ProcessInput on the Character
                    animatedUnit->ProcessEvent(event);
                }
            }
        }
    }
}

void UnitManager::SetUnitZOrder(unsigned int id, int zOrder)
{
    Unit* unit = GetUnit(id);
    if (unit)
    {
        // Attempt to cast to AnimatedUnit to set Z-order
        if (AnimatedUnit* animatedUnit = dynamic_cast<AnimatedUnit*>(unit))
        {
            animatedUnit->SetZOrder(zOrder);
            m_needsSorting = true;  // Mark for resorting
        }
        // else: Unit is not animated, cannot set Z-order - optionally log warning
    }
}

void UnitManager::BringUnitToFront(unsigned int id)
{
    int highestZOrder = 0;  // Default Z-Order is effectively 0 for non-animated

    // Find the current highest Z-order among AnimatedUnits
    for (const auto& unitPtr : m_units)
    {
        if (unitPtr == nullptr)
            continue;  // Skip null pointers
        // Attempt to cast to AnimatedUnit to get Z-order
        if (const AnimatedUnit* animatedUnit = dynamic_cast<const AnimatedUnit*>(unitPtr.get()))
        {
            highestZOrder = std::max(highestZOrder, animatedUnit->GetZOrder());
        }
    }

    // Set the target unit's Z-order to be one higher than the current max
    // SetUnitZOrder handles the cast and marks for sorting
    SetUnitZOrder(id, highestZOrder + 1);
}

void UnitManager::Clear()
{
    // Clearing the vector destroys the unique_ptrs and thus the Units
    m_units.clear();
    // Clear the lookup maps
    m_unitsById.clear();
    m_unitsByName.clear();
    m_needsSorting = false;
}

void UnitManager::AddUnitImmediate(std::unique_ptr<Unit> unit)
{
    if (!unit)
        return;

    unsigned int id     = unit->GetId();
    std::string  name   = unit->GetName();
    Unit*        rawPtr = unit.get();

    // 1) store in lookup maps
    m_unitsById[id] = rawPtr;
    if (!name.empty())
        m_unitsByName[name] = rawPtr;

    // 2) store in main vector
    m_units.push_back(std::move(unit));
    m_needsSorting = true;
}

void UnitManager::RemoveUnitImmediate(unsigned int id)
{
    // 1) remove from lookup maps
    auto itId = m_unitsById.find(id);
    if (itId != m_unitsById.end())
    {
        auto* raw = itId->second;
        auto  nm  = raw->GetName();
        m_unitsById.erase(itId);
        if (!nm.empty())
            m_unitsByName.erase(nm);
    }

    // 2) remove from main vector
    auto it =
        std::find_if(m_units.begin(), m_units.end(), [id](auto& u) { return u->GetId() == id; });
    if (it != m_units.end())
    {
        m_units.erase(it);
        // no need to set m_needsSorting here
    }
}

size_t UnitManager::GetUnitCount() const
{
    return m_units.size();
}

void UnitManager::SortUnitsByZOrder()
{
    std::stable_sort(m_units.begin(),
                     m_units.end(),
                     [](const std::unique_ptr<Unit>& a, const std::unique_ptr<Unit>& b) -> bool {
                         // Default Z-order for non-animated units
                         int zOrderA = 0;
                         int zOrderB = 0;

                         // Attempt to get Z-order if they are AnimatedUnits
                         if (const AnimatedUnit* animA = dynamic_cast<const AnimatedUnit*>(a.get()))
                         {
                             zOrderA = animA->GetZOrder();
                         }
                         if (const AnimatedUnit* animB = dynamic_cast<const AnimatedUnit*>(b.get()))
                         {
                             zOrderB = animB->GetZOrder();
                         }

                         // Sort primarily by Z-order (ascending)
                         if (zOrderA != zOrderB)
                         {
                             return zOrderA < zOrderB;
                         }

                         // --- Optional: Secondary sort by Y position for units with the same
                         // Z-order --- This helps with pseudo-3D layering where units lower on
                         // screen draw on top. return a->GetPosition().y < b->GetPosition().y;

                         // If Z-orders are equal and no secondary sort, maintain relative order
                         // (stable_sort)
                         return false;
                     });

    // After sorting, update the non-owning pointers in maps if necessary,
    // although generally not needed if only vector order matters for draw.
    // If lookup maps become invalid after sort (they shouldn't with stable_sort),
    // they would need rebuilding here.
}