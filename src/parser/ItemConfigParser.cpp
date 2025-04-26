#include "parser/ItemConfigParser.hpp"

bool ItemConfigParser::ParseFromFile(const std::string& filename) {
    // 8 tokens per line
    return ConfigParserUtils::ReadTokensFile(
        filename,
        m_data,
        /* expectedCols = */ 8,
        m_lastError
    );
}