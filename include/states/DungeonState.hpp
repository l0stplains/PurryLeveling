#pragma once

#include "systems/CheatConsole.hpp"

#include "ui/BattleUnitInfo.hpp"
#include "ui/BossHealthBar.hpp"
#include "ui/Button.hpp"
#include "ui/CharacterInfo.hpp"
#include "ui/MobInfo.hpp"
#include "ui/QuestInfo.hpp"

#include "states/State.hpp"
#include "units/summons/Summon.hpp"
#include "units/summons/Wildfire.hpp"

class DungeonState : public State
{
public:
    /**
     * @brief Constructor
     *
     * @param context The game context to share resources
     */
    explicit DungeonState(GameContext& context, DimensionType dimension, Dungeon& dungeon);

    void Init() override;

    /**
     * @brief Handle input events
     *
     * @param event The SFML event to handle
     * @param window The SFML window
     * @return State::StateChange The state change information
     */
    State::StateChange ProcessEvent(const sf::Event& event) override;

    State::StateChange Update(const sf::Time& dt) override;

    void Draw(sf::RenderWindow& window) override;

    void RenderUI() override;

    void Exit() override;  // Important for thread cleanup!
    void Pause() override;
    void Resume() override;

private:
    sf::Sound m_bossBacksound;

    sf::Texture m_backgroundTexture;
    sf::Texture m_bossBackgroundTexture;
    sf::Sprite  m_backgroundSprite;
    sf::Sprite  m_chamberExitArea;

    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound

    sf::Texture m_buttonTexture;  ///< Button texture

    Button m_attackButton;  ///< attack  button
    Button m_useSkillButton;
    Button m_useItemButton;  ///< use item button
    Button m_exitButton;     ///< Start button

    sf::Font m_font;      ///< Font for UI text
    sf::Font m_boldFont;  ///< Font for UI text

    NavigationGrid m_navGrid;     ///< Navigation grid for unit movement
    NavigationGrid m_mobNavGrid;  ///< Navigation grid for mob movement

    BattleUnitInfo m_battleUnitInfo;  ///< Battle unit information UI
    QuestInfo      m_questInfo;       ///< Quest information UI
    MobInfo        m_mobInfo;         ///< Mob information UI
    CharacterInfo  m_characterInfo;   ///< Character information UI

    bool m_isPlayerTurn     = true;
    bool m_wasInChamberExit = false;

    bool m_triggerActionTurn = false;

    bool     m_showExitPopup = false;  ///< Flag to show exit confirmation popup
    Dungeon  m_dungeon;
    Chamber* m_chamber;

    Pointer       m_chamberExitPointer;
    BossHealthBar m_bossHealthBar;

    std::queue<unsigned int> m_turnQueue;

    AnimatedUnit*             m_character;
    std::vector<unsigned int> m_mobsID;
    State::StateChange        m_pendingStateChange;  ///< Pending state change information

    bool m_isTransitioning = false;
    bool m_walkToExit      = false;

    CheatConsole m_cheatConsole;

    void playMobTurn();

    void nextChamber();

    void initializeCheat();

    std::vector<sf::Vector2f> generateMobSpawnPoints(const sf::Vector2f& center,
                                                     unsigned int        mobCount,
                                                     bool                isLeftHalf);
    template <typename UnitType>
    UnitType* getClosestUnitOfType(const std::vector<unsigned int>& ids, const sf::Vector2f& targetPos)
    {
        UnitType* closest  = nullptr;
        float     bestDist = std::numeric_limits<float>::max();

        for (auto id : ids)
        {
            // Lookup the unit of this type
            UnitType* unit = GetContext().GetUnitManager()->GetUnitOfType<UnitType>(id);

            if (!unit || !unit->IsActive())
                continue;

            AnimatedUnit* animatedUnit =
                GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(id);

            if (animatedUnit && animatedUnit->GetZOrder() != 0)
                continue;

            sf::Vector2f diff = animatedUnit->GetPosition() - targetPos;
            float        dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            if (dist < bestDist)
            {
                bestDist = dist;
                closest  = unit;
            }
        }

        return closest;
    }
};