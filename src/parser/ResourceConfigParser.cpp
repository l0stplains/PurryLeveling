
#include "parser/ResourceConfigParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "exception/Exception.hpp"

// Initialize static member
std::vector<ResourceConfigParser::ResourceEntry> ResourceConfigParser::m_emptyVector;

std::string ResourceConfigParser::Trim(const std::string& str) const
{
    if (str.empty())
        return str;

    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

void ResourceConfigParser::ParseFromFile(const std::string& filename)
{
    // Clear any previous data
    m_resources.clear();
    m_lastError.clear();

    std::ifstream file(filename);
    if (!file.is_open())
    {
        m_lastError = "Failed to open resource config file: " + filename;
        throw FileNotFoundException(filename, m_lastError);
    }

    std::string line;
    std::string currentCategory;
    int         lineNumber = 0;

    try
    {
        while (std::getline(file, line))
        {
            lineNumber++;

            // Skip empty lines and comments
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            // Check if this is a category header
            if (line[0] == '[' && line.back() == ']')
            {
                currentCategory = line.substr(1, line.size() - 2);
                continue;
            }

            // Skip if no category is defined yet
            if (currentCategory.empty())
            {
                continue;
            }

            // Parse the resource entry (id = path)
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos)
            {
                ResourceEntry entry;

                // Extract and trim ID
                entry.id = Trim(line.substr(0, equalPos));
                if (entry.id.empty())
                {
                    m_lastError = "Line " + std::to_string(lineNumber) + ": Empty resource ID";
                }

                // Extract and trim path
                entry.path = Trim(line.substr(equalPos + 1));
                if (entry.path.empty())
                {
                    m_lastError = "Line " + std::to_string(lineNumber) +
                                  ": Empty resource path for ID '" + entry.id + "'";
                    throw ResourceNotFoundException(filename, m_lastError);
                }

                // Add to resources
                m_resources[currentCategory].push_back(entry);
            }
            else
            {
                // Line doesn't contain an equal sign
                m_lastError =
                    "Line " + std::to_string(lineNumber) + ": Invalid format, expected 'id = path'";
                throw LineTooShortException(filename, m_lastError);
            }
        }
    }
    catch (const std::exception& e)
    {
        m_lastError = "Error parsing line " + std::to_string(lineNumber) + ": " + e.what();
        throw InvalidFormatException(filename, m_lastError);
    }
}

const std::vector<ResourceConfigParser::ResourceEntry>& ResourceConfigParser::GetResources(
    const std::string& category) const
{
    auto it = m_resources.find(category);
    if (it != m_resources.end())
    {
        return it->second;
    }
    return m_emptyVector;
}

bool ResourceConfigParser::HasCategory(const std::string& category) const
{
    return m_resources.find(category) != m_resources.end();
}