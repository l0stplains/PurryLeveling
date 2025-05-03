#pragma once

#include <string>

#include "exception/Exception.hpp"

/**
 * @class ConfigParser
 * @brief Abstract base class for configuration parsers
 *
 * This class defines the interface for parsing configuration files.
 * Different implementations can handle different file formats and structures.
 */
class ConfigParser
{
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup in derived classes
     */
    virtual ~ConfigParser() = default;

    /**
     * @brief Parse configuration from a file
     * @param filename Path to the configuration file
     * @return true if parsing was successful, false otherwise
     */
    virtual void ParseFromFile(const std::string& filename) = 0;

    /**
     * @brief Get the last error message
     * @return The last error message that occurred during parsing
     */
    virtual std::string GetLastError() const = 0;

    /**
     * @brief Set the last error message
     * @param error The error message to set
     */
    void SetLastError(const std::string& error) { m_lastError = error; }

protected:
    /**
     * @brief Store the last error that occurred during parsing
     */
    std::string m_lastError;
};