#include "parser/ShopConfigParser.hpp"

void ShopConfigParser::ParseFromFile(const std::string& filename)
{
    // 4 tokens per line
    if (!ConfigParserUtils::ReadTokensFile(filename + "/shop.txt",
                                           m_data,
                                           /* expectedCols = */ 4,
                                           m_lastError))
    {
        throw FileNotFoundException(filename, m_lastError);
    }

    // check for empty lines
}