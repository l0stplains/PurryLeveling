#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

/**
 * @brief Manages the walkable areas of the game world.
 */
class NavigationGrid
{
public:
    /**
     * @brief Constructor.
     * @param worldWidth Total width of the game world (window width).
     * @param worldHeight Total height of the game world (window height).
     * @param tileWidth Width of a single grid tile.
     * @param tileHeight Height of a single grid tile.
     */
    NavigationGrid(unsigned int worldWidth,
                   unsigned int worldHeight,
                   unsigned int tileWidth,
                   unsigned int tileHeight);

    /**
     * @brief Sets whether a specific tile is walkable.
     * @param gridX X-coordinate of the tile in the grid.
     * @param gridY Y-coordinate of the tile in the grid.
     * @param isWalkable True if the tile should be walkable, false otherwise.
     */
    void SetTileWalkable(unsigned int gridX, unsigned int gridY, bool isWalkable);

    /**
     * @brief Replace the internal grid with a brand-new one.
     * @param newGrid A 2D vector of bools.
     * @return true if newGrid had exactly the same dimensions as this grid and was accepted;
     *         false otherwise (and leaves the old grid untouched).
     */
    bool SetGrid(const std::vector<std::vector<bool>>& newGrid);

    /**
     * @brief Checks if a specific world position is walkable.
     * @param worldPos The position in world coordinates.
     * @return True if the position corresponds to a walkable tile and is within world bounds, false
     * otherwise.
     */
    bool IsWalkable(const sf::Vector2f& worldPos) const;

    /**
     * @brief Converts world coordinates to grid coordinates.
     * @param worldPos The position in world coordinates.
     * @return The corresponding grid coordinates.
     */
    sf::Vector2u WorldToGridCoords(const sf::Vector2f& worldPos) const;

    /**
     * @brief Gets the width of the grid in tiles.
     */
    unsigned int GetGridWidth() const { return m_gridWidth; }

    /**
     * @brief Gets the height of the grid in tiles.
     */
    unsigned int GetGridHeight() const { return m_gridHeight; }

    /**
     * @brief Gets the width of a single tile.
     */
    unsigned int GetTileWidth() const { return m_tileWidth; }

    /**
     * @brief Gets the height of a single tile.
     */
    unsigned int GetTileHeight() const { return m_tileHeight; }

    sf::Vector2f GridToWorldCoords(const sf::Vector2u& gridCoords) const;

    bool FindClosestWalkable(const sf::Vector2f& worldPos, sf::Vector2f& outPosition) const;

private:
    unsigned int                   m_worldWidth;
    unsigned int                   m_worldHeight;
    unsigned int                   m_tileWidth;
    unsigned int                   m_tileHeight;
    unsigned int                   m_gridWidth;
    unsigned int                   m_gridHeight;
    std::vector<std::vector<bool>> m_grid;  // true = walkable, false = blocked
};