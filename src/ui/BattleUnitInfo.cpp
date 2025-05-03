#include "ui/BattleUnitInfo.hpp"

BattleUnitInfo::BattleUnitInfo(GameContext& gameContext, float windowWidth, float windowHeight)
    : m_windowWidth(windowWidth),
      m_windowHeight(windowHeight),
      m_gameContext(gameContext),
      m_texture(gameContext.GetResourceManager()->GetTexture("main_logo"))  // Default texture
{
    // Initialize texture that will be used to display the sprite
}

// Update the sprite texture from the unit's spritesheet
void BattleUnitInfo::updateSpriteTexture(const AnimatedUnit& unit)
{
    const sf::Texture& texture     = unit.GetTextures(UnitAnimationType::IDLE);
    sf::Image          spritesheet = texture.copyToImage();
    if (spritesheet.getSize().x > 0 && spritesheet.getSize().y > 0)
    {
        // Create a temporary image to hold our cropped sprite
        sf::Image croppedImage;
        croppedImage.resize({32, 32}, {80, 80, 80, 160});

        // Copy the portion of the spritesheet to our new image (fixed at 0,0)
        for (int y = 0; y < 32; y++)
        {
            for (int x = 0; x < 32; x++)
            {
                // Fixed position at (0,0)
                int sourceX = 0 + x;
                int sourceY = 0 + y;

                if (sourceX < spritesheet.getSize().x && sourceY < spritesheet.getSize().y)
                {
                    croppedImage.setPixel({x, y}, spritesheet.getPixel({sourceX, sourceY}));
                    croppedImage.setPixel({x, y}, spritesheet.getPixel({sourceX, sourceY}));
                }
            }
        }

        // Update the texture with our cropped image
        m_texture.update(croppedImage);
    }
}

// Render the unit info window
void BattleUnitInfo::render(const AnimatedUnit& unit)
{
    // Update the sprite texture
    updateSpriteTexture(unit);

    // Get viewport dimensions for positioning
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 windowPos(viewport->WorkPos.x + viewport->WorkSize.x - m_windowWidth - 10,  // Bottom
                                                                                       // right
                     viewport->WorkPos.y + viewport->WorkSize.y - m_windowHeight - 10);

    // Set window position and size
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiCond_Always);

    // Window flags to make it non-movable, non-resizable, and non-collapsable
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

    // Begin the window
    if (ImGui::Begin("Unit Info", nullptr, flags))
    {
        // Display unit name
        ImGui::Text("%s", unit.GetName().c_str());
        /*
        if (!unit.getStatus().empty()) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "[%s]", unit.getStatus().c_str());
        }
            */

        ImGui::Separator();

        // Display the sprite
        ImGui::BeginChild("sprite_area", ImVec2(m_windowWidth * 0.9f, m_windowWidth * 0.9f), true);
        ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - 64) * 0.5f);
        ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - 64) * 0.5f);
        ImTextureID textureId = (ImTextureID)(intptr_t)m_texture.getNativeHandle();
        ImGui::Image(textureId, ImVec2(64, 64));
        ImGui::EndChild();

        // HP Bar
        ImGui::Text("HP: %d/%d", unit.GetHealth(), unit.GetMaxHealth());
        float hpRatio = unit.GetMaxHealth() > 0 ? static_cast<float>(unit.GetHealth()) /
                                                      static_cast<float>(unit.GetMaxHealth())
                                                : 0.0f;
        ImGui::ProgressBar(hpRatio, ImVec2(-1, 0), "");

        // Mana Bar
        ImGui::Text("Mana: %d/%d", unit.GetCurrentMana(), unit.GetMaxMana());
        float manaRatio = unit.GetMaxMana() > 0 ? static_cast<float>(unit.GetCurrentMana()) /
                                                      static_cast<float>(unit.GetMaxMana())
                                                : 0.0f;
        ImGui::ProgressBar(manaRatio, ImVec2(-1, 0), "");

        // Optional description in a scrollable area
        /*
        if (!unit.getDescription().empty()) {
            ImGui::Separator();
            ImGui::Text("Description:");
            ImGui::BeginChild("description", ImVec2(0, 80), true);
            ImGui::TextWrapped("%s", unit.getDescription().c_str());
            ImGui::EndChild();
        }
            */
    }
    ImGui::End();
}