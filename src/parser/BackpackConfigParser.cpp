#include "parser/BackpackConfigParser.hpp"

bool BackpackConfigParser::ParseFromFile(const std::string& filename) {
    // 4 tokens per line
    return ConfigParserUtils::ReadTokensFile(
        filename,
        m_data,
        /* expectedCols = */ 4,
        m_lastError
    );
}