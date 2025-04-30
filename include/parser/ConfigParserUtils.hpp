// ConfigParserUtils.hpp
#pragma once

#include <string>
#include <vector>

/**
 * @class ConfigParserUtils
 * @brief Utility class for parsing whitespace-delimited config files
 *        with comment stripping and column/row validation.
 */
class ConfigParserUtils {
public:
    /**
     * @brief Read a whitespace-delimited config file (ignoring lines starting with '#').
     * @param filename    Path to the file to read
     * @param outRecords  Filled with each non-empty, non-comment line as a vector of tokens
     * @param lastError   On failure, set to the error message
     * @return true on success, false on open-error or I/O error
     */
    static bool ReadTokensFile(
        const std::string& filename,
        std::vector<std::vector<std::string>>& outRecords,
        std::string& lastError
    );

    /**
     * @brief Read file and ensure each row has exactly expectedCols tokens
     */
    static bool ReadTokensFile(
        const std::string& filename,
        std::vector<std::vector<std::string>>& outRecords,
        size_t expectedCols,
        std::string& lastError
    );

    /**
     * @brief Read file and ensure it has expectedRows rows of expectedCols tokens
     */
    static bool ReadTokensFile(
        const std::string& filename,
        std::vector<std::vector<std::string>>& outRecords,
        size_t expectedCols,
        size_t expectedRows,
        std::string& lastError
    );

private:
    // Helper to trim whitespace from both ends
    static std::string Trim(const std::string& s);
};