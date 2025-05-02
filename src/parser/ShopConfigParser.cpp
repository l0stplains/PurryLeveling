#include "parser/ShopConfigParser.hpp"

void ShopConfigParser::ParseFromFile(const std::string& filename)
{
    // 4 tokens per line
    if (!ConfigParserUtils::ReadTokensFile(filename,
                                           m_data,
                                           /* expectedCols = */ 4,
                                           m_lastError))
    {
        throw FileNotFoundException();
    }

    // check for empty lines
}