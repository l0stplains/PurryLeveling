#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

/**
 * @class ResourceManager
 * @brief Manages loading and retrieving game resources such as textures, fonts, and sound buffers.
 *
 * This class provides methods to load resources from files, retrieve them by ID,
 * check if they exist, and unload them when no longer needed.
 */
class ResourceManager
{
public:
    /**
     * @brief Default constructor
     */
    ResourceManager();

    /**
     * @brief Destructor
     */
    ~ResourceManager();

    // Texture loading and retrieval

    /**
     * @brief Load a texture from a file and associate it with an ID
     *
     * @param id Unique identifier for the texture
     * @param filepath Path to the texture file
     */
    void LoadTexture(const std::string& id, const std::string& filepath);

    /**
     * @brief Get a reference to a loaded texture by its ID
     *
     * @param id Unique identifier for the texture
     * @return Reference to the loaded texture
     * @throws std::runtime_error if the texture is not found
     */
    sf::Texture& GetTexture(const std::string& id);

    /**
     * @brief Check if a texture with the given ID exists
     *
     * @param id Unique identifier for the texture
     * @return true if the texture exists, false otherwise
     */
    bool HasTexture(const std::string& id) const;

    /**
     * @brief Unload a texture by its ID
     *
     * @param id Unique identifier for the texture
     */
    void UnloadTexture(const std::string& id);

    // Font loading and retrieval

    /**
     * @brief Load a font from a file and associate it with an ID
     *
     * @param id Unique identifier for the font
     * @param filepath Path to the font file
     */
    void LoadFont(const std::string& id, const std::string& filepath);

    /**
     * @brief Get a reference to a loaded font by its ID
     *
     * @param id Unique identifier for the font
     * @return Reference to the loaded font
     * @throws std::runtime_error if the font is not found
     */
    sf::Font& GetFont(const std::string& id);

    /**
     * @brief Check if a font with the given ID exists
     *
     * @param id Unique identifier for the font
     * @return true if the font exists, false otherwise
     */
    bool HasFont(const std::string& id) const;

    /**
     * @brief Unload a font by its ID
     *
     * @param id Unique identifier for the font
     */
    void UnloadFont(const std::string& id);

    // Sound buffer loading and retrieval

    /**
     * @brief Load a sound buffer from a file and associate it with an ID
     *
     * @param id Unique identifier for the sound buffer
     * @param filepath Path to the sound file
     */
    void LoadSoundBuffer(const std::string& id, const std::string& filepath);

    /**
     * @brief Get a reference to a loaded sound buffer by its ID
     *
     * @param id Unique identifier for the sound buffer
     * @return Reference to the loaded sound buffer
     * @throws std::runtime_error if the sound buffer is not found
     */
    sf::SoundBuffer& GetSoundBuffer(const std::string& id);

    /**
     * @brief Check if a sound buffer with the given ID exists
     *
     * @param id Unique identifier for the sound buffer
     * @return true if the sound buffer exists, false otherwise
     */
    bool HasSoundBuffer(const std::string& id) const;

    /**
     * @brief Unload a sound buffer by its ID
     *
     * @param id Unique identifier for the sound buffer
     */
    void UnloadSoundBuffer(const std::string& id);

    // Utility methods

    /**
     * @brief Clear all loaded resources
     *
     * This method clears all loaded textures, fonts, and sound buffers.
     */
    void ClearAll();

private:
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;   ///< Map of texture ID to
                                                                      ///< texture
    std::map<std::string, std::unique_ptr<sf::Font>>        m_fonts;  ///< Map of font ID to font
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;  ///< Map of sound
                                                                             ///< buffer ID to sound
                                                                             ///< buffer
};
