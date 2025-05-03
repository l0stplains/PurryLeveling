#pragma once

#include <map>
#include <string>
#include <vector>

#include "ConfigParser.hpp"

/**
 * @class ResourceConfigParser
 * @brief Parser for game resource configuration files
 *
 * This class parses resource configuration files that contain
 * category-based resource definitions with IDs and file paths.
 *
 * @see ConfigParser
 */
class ResourceConfigParser : public ConfigParser
{
public:
    /**
     * @brief Resource entry - simple ID and path pair
     */
    struct ResourceEntry
    {
        std::string id;
        std::string path;
    };

    /**
     * @brief Parse a resource configuration file.
     * Format:
     * # Comments start with #
     * [category]
     * id = path
     */
    void ParseFromFile(const std::string& filename) override;

    /**
     * @brief Get all resources belonging to a category
     * @param category The category to retrieve resources for
     * @return const reference to vector of resource entries
     */
    const std::vector<ResourceEntry>& GetResources(const std::string& category) const;

    /**
     * @brief Check if a category exists in the parsed configuration
     * @param category The category name to check
     * @return true if the category exists, false otherwise
     */
    bool HasCategory(const std::string& category) const;

    /**
     * @brief Get the last error message
     * @return The last error message that occurred during parsing
     */
    std::string GetLastError() const override { return m_lastError; }

private:
    /**
     * @brief Map of category -> vector of resource entries
     */
    std::map<std::string, std::vector<ResourceEntry>> m_resources;

    /**
     * @brief Empty vector to return when category doesn't exist
     */
    static std::vector<ResourceEntry> m_emptyVector;

    /**
     * @brief Helper method to trim whitespace from a string
     *
     * @param str The string to trim
     * @return The trimmed string
     */
    std::string Trim(const std::string& str) const;
};