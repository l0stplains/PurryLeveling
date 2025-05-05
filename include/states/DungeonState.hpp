#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>
#include <queue>
#include <vector>

#include "core/GameContext.hpp"

#include "systems/CheatConsole.hpp"

#include "ui/BattleUnitInfo.hpp"
#include "ui/BossHealthBar.hpp"
#include "ui/Button.hpp"
#include "ui/CharacterInfo.hpp"
#include "ui/QuestInfo.hpp"

#include "dungeon/Dungeon.hpp"
#include "dungeon/DungeonFactory.hpp"
#include "props/Pointer.hpp"
#include "props/Portal.hpp"
#include "states/Enums.hpp"
#include "states/State.hpp"
#include "units/NavigationGrid.hpp"

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

    void Exit() override;
    void Pause() override;
    void Resume() override;

private:
    sf::Texture m_backgroundTexture;
    sf::Texture m_bossBackgroundTexture;
    sf::Sprite  m_backgroundSprite;
    sf::Sprite  m_chamberExitArea;

    sf::Sound m_buttonHoverSound;
    sf::Sound m_buttonClickSound;

    sf::Texture m_buttonTexture;

    Button m_attackButton;
    Button m_useSkillButton;
    Button m_useItemButton;
    Button m_exitButton;

    sf::Font m_font;
    sf::Font m_boldFont;

    NavigationGrid m_navGrid;
    NavigationGrid m_mobNavGrid;

    BattleUnitInfo m_battleUnitInfo;
    CharacterInfo  m_characterInfo;
    QuestInfo      m_questInfo;

    bool m_isPlayerTurn     = true;
    bool m_wasInChamberExit = false;

    bool m_triggerActionTurn = false;

    bool     m_showExitPopup = false;
    Dungeon  m_dungeon;
    Chamber* m_chamber;

    Pointer       m_chamberExitPointer;
    BossHealthBar m_bossHealthBar;

    std::queue<unsigned int> m_turnQueue;

    AnimatedUnit*             m_character;
    std::vector<unsigned int> m_mobsID;
    State::StateChange        m_pendingStateChange;

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