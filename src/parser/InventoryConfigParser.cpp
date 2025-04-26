#include "parser/InventoryConfigParser.hpp"

bool InventoryConfigParser::ParseFromFile(const std::string& filename) {
    // 2 tokens per line, exactly 5 lines
    return ConfigParserUtils::ReadTokensFile(
        filename,
        m_data,
        /* expectedCols = */ 2,
        /* expectedRows = */ 5,
        m_lastError
    );
}
