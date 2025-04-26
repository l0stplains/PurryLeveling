#include "parser/ConfigParserUtils.hpp"
#include <fstream>
#include <sstream>

std::string ConfigParserUtils::Trim(const std::string& s) {
    auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    auto e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

bool ConfigParserUtils::ReadTokensFile(
    const std::string& filename,
    std::vector<std::vector<std::string>>& outRecords,
    std::string& lastError
) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        lastError = "Failed to open file: " + filename;
        return false;
    }
    std::string line;
    size_t lineNo = 0;
    while (std::getline(file, line)) {
        ++lineNo;
        // strip comments
        if (auto pos = line.find('#'); pos != std::string::npos)
            line.erase(pos);
        line = Trim(line);
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) tokens.push_back(std::move(tok));
        outRecords.push_back(std::move(tokens));
    }
    return true;
}

bool ConfigParserUtils::ReadTokensFile(
    const std::string& filename,
    std::vector<std::vector<std::string>>& outRecords,
    size_t expectedCols,
    std::string& lastError
) {
    if (!ReadTokensFile(filename, outRecords, lastError))
        return false;
    for (size_t i = 0; i < outRecords.size(); ++i) {
        if (outRecords[i].size() != expectedCols) {
            lastError = "Line " + std::to_string(i+1) + ": expected "
                      + std::to_string(expectedCols) + " columns, got "
                      + std::to_string(outRecords[i].size());
            return false;
        }
    }
    return true;
}

bool ConfigParserUtils::ReadTokensFile(
    const std::string& filename,
    std::vector<std::vector<std::string>>& outRecords,
    size_t expectedCols,
    size_t expectedRows,
    std::string& lastError
) {
    if (!ReadTokensFile(filename, outRecords, expectedCols, lastError))
        return false;
    if (outRecords.size() != expectedRows) {
        lastError = "Expected " + std::to_string(expectedRows)
                  + " rows, got " + std::to_string(outRecords.size());
        return false;
    }
    return true;
}