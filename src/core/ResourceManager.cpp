// ResourceManager.cpp
#include "core/ResourceManager.hpp"

#include <SFML/Graphics/Font.hpp>

ResourceManager::ResourceManager()
{
    // Constructor - could initialize default resources if needed
}
ResourceManager::~ResourceManager()
{
    // Resources will be automatically cleaned up by unique_ptr
}
void ResourceManager::LoadTexture(const std::string& id, const std::string& filepath)
{
    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filepath))
    {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }

    m_textures[id] = std::move(texture);
}
sf::Texture& ResourceManager::GetTexture(const std::string& id)
{
    auto found = m_textures.find(id);
    if (found == m_textures.end())
    {
        throw std::runtime_error("Texture not found: " + id);
    }
    return *found->second;
}
bool ResourceManager::HasTexture(const std::string& id) const
{
    return m_textures.find(id) != m_textures.end();
}
void ResourceManager::UnloadTexture(const std::string& id)
{
    auto found = m_textures.find(id);
    if (found != m_textures.end())
    {
        m_textures.erase(found);
    }
}
void ResourceManager::LoadFont(const std::string& id, const std::string& filepath)
{
    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
    if (!font->openFromFile(filepath))
    {
        throw std::runtime_error("Failed to load font: " + filepath);
    }

    m_fonts[id] = std::move(font);
}
sf::Font& ResourceManager::GetFont(const std::string& id)
{
    auto found = m_fonts.find(id);
    if (found == m_fonts.end())
    {
        throw std::runtime_error("Font not found: " + id);
    }
    return *found->second;
}
bool ResourceManager::HasFont(const std::string& id) const
{
    return m_fonts.find(id) != m_fonts.end();
}
void ResourceManager::UnloadFont(const std::string& id)
{
    auto found = m_fonts.find(id);
    if (found != m_fonts.end())
    {
        m_fonts.erase(found);
    }
}
void ResourceManager::LoadSoundBuffer(const std::string& id, const std::string& filepath)
{
    std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filepath))
    {
        throw std::runtime_error("Failed to load sound: " + filepath);
    }

    m_soundBuffers[id] = std::move(buffer);
}
sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& id)
{
    auto found = m_soundBuffers.find(id);
    if (found == m_soundBuffers.end())
    {
        throw std::runtime_error("Sound buffer not found: " + id);
    }
    return *found->second;
}
bool ResourceManager::HasSoundBuffer(const std::string& id) const
{
    return m_soundBuffers.find(id) != m_soundBuffers.end();
}
void ResourceManager::UnloadSoundBuffer(const std::string& id)
{
    auto found = m_soundBuffers.find(id);
    if (found != m_soundBuffers.end())
    {
        m_soundBuffers.erase(found);
    }
}
void ResourceManager::ClearAll()
{
    m_textures.clear();
    m_fonts.clear();
    m_soundBuffers.clear();
}