#include "parser/MobLootConfigParser.hpp"

bool MobLootConfigParser::ParseFromFile(const std::string& filename) {
    // 3 tokens per line
    return ConfigParserUtils::ReadTokensFile(
        filename,
        m_data,
        /* expectedCols = */ 3,
        m_lastError
    );
}