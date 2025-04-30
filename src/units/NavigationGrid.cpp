#include "units/NavigationGrid.hpp"

#include <cmath>      // For floor
#include <stdexcept>  // For runtime_error

NavigationGrid::NavigationGrid(unsigned int worldWidth,
                               unsigned int worldHeight,
                               unsigned int tileWidth,
                               unsigned int tileHeight)
    : m_worldWidth(worldWidth),
      m_worldHeight(worldHeight),
      m_tileWidth(tileWidth),
      m_tileHeight(tileHeight)
{
    if (tileWidth == 0 || tileHeight == 0)
    {
        throw std::runtime_error("Tile dimensions cannot be zero.");
    }
    m_gridWidth = worldWidth / tileWidth + (worldWidth % tileWidth > 0 ? 1 : 0);  // Ensure full
                                                                                  // coverage
    m_gridHeight = worldHeight / tileHeight + (worldHeight % tileHeight > 0 ? 1 : 0);

    // Initialize grid - all walkable by default
    m_grid.resize(m_gridHeight, std::vector<bool>(m_gridWidth, true));
}

void NavigationGrid::SetTileWalkable(unsigned int gridX, unsigned int gridY, bool isWalkable)
{
    if (gridY < m_gridHeight && gridX < m_gridWidth)
    {
        m_grid[gridY][gridX] = isWalkable;
    }
    // Optionally add logging or error handling for out-of-bounds access
}

bool NavigationGrid::SetGrid(const std::vector<std::vector<bool>>& newGrid)
{
    // check number of rows
    if (newGrid.size() != m_gridHeight)
        return false;

    // check each row’s length
    for (const auto& row : newGrid)
    {
        if (row.size() != m_gridWidth)
            return false;
    }

    // all good: swap in the new grid
    m_grid = newGrid;
    return true;
}

sf::Vector2u NavigationGrid::WorldToGridCoords(const sf::Vector2f& worldPos) const
{
    // Ensure coordinates are within bounds before division
    float clampedX = std::max(0.0f, std::min(worldPos.x, (float)m_worldWidth - 1));
    float clampedY = std::max(0.0f, std::min(worldPos.y, (float)m_worldHeight - 1));

    unsigned int gridX = static_cast<unsigned int>(std::floor(clampedX / m_tileWidth));
    unsigned int gridY = static_cast<unsigned int>(std::floor(clampedY / m_tileHeight));

    // Clamp grid coordinates to be within the valid grid range
    gridX = std::min(gridX, m_gridWidth - 1);
    gridY = std::min(gridY, m_gridHeight - 1);

    return {gridX, gridY};
}

bool NavigationGrid::IsWalkable(const sf::Vector2f& worldPos) const
{
    // 1. Check world boundaries
    if (worldPos.x < 0.0f || worldPos.x >= m_worldWidth || worldPos.y < 0.0f ||
        worldPos.y >= m_worldHeight)
    {
        return false;
    }

    // 2. Convert to grid coordinates
    sf::Vector2u gridCoords = WorldToGridCoords(worldPos);

    // 3. Check grid walkability (safe due to previous checks and WorldToGridCoords clamping)
    return m_grid[gridCoords.y][gridCoords.x];
}

sf::Vector2f NavigationGrid::GridToWorldCoords(const sf::Vector2u& gridCoords) const
{
    float x = gridCoords.x * static_cast<float>(m_tileWidth) + m_tileWidth * 0.5f;
    float y = gridCoords.y * static_cast<float>(m_tileHeight) + m_tileHeight * 0.5f;
    return {x, y};
}

bool NavigationGrid::FindClosestWalkable(const sf::Vector2f& worldPos, sf::Vector2f& outPosition) const
{
    sf::Vector2u center    = WorldToGridCoords(worldPos);
    unsigned int maxRadius = std::max(m_gridWidth, m_gridHeight);
    for (unsigned int r = 1; r <= maxRadius; ++r)
    {
        int minX = static_cast<int>(center.x) - static_cast<int>(r);
        int maxX = static_cast<int>(center.x) + static_cast<int>(r);
        int minY = static_cast<int>(center.y) - static_cast<int>(r);
        int maxY = static_cast<int>(center.y) + static_cast<int>(r);
        // Check horizontal edges
        for (int x = minX; x <= maxX; ++x)
        {
            for (int y : {minY, maxY})
            {
                if (x < 0 || y < 0 || x >= static_cast<int>(m_gridWidth) ||
                    y >= static_cast<int>(m_gridHeight))
                    continue;
                if (m_grid[y][x])
                {
                    outPosition = GridToWorldCoords(
                        {static_cast<unsigned int>(x), static_cast<unsigned int>(y)});
                    return true;
                }
            }
        }
        // Check vertical edges
        for (int y = minY + 1; y < maxY; ++y)
        {
            for (int x : {minX, maxX})
            {
                if (x < 0 || y < 0 || x >= static_cast<int>(m_gridWidth) ||
                    y >= static_cast<int>(m_gridHeight))
                    continue;
                if (m_grid[y][x])
                {
                    outPosition = GridToWorldCoords(
                        {static_cast<unsigned int>(x), static_cast<unsigned int>(y)});
                    return true;
                }
            }
        }
    }
    return false;
}