#include "parser/ShopConfigParser.hpp"

bool ShopConfigParser::ParseFromFile(const std::string& filename) {
    // 4 tokens per line
    return ConfigParserUtils::ReadTokensFile(
        filename,
        m_data,
        /* expectedCols = */ 4,
        m_lastError
    );
}