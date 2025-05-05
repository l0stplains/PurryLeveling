#include "states/DungeonState.hpp"

// Constructor
DungeonState::DungeonState(GameContext& context, DimensionType dimension, Dungeon& dungeon)
    : State(context),
      m_bossBacksound(GetContext().GetResourceManager()->GetSoundBuffer("boss_backsound")),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("world_background")),
      m_bossBackgroundTexture(GetContext().GetResourceManager()->GetTexture("world_background")),
      m_backgroundSprite(m_backgroundTexture),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_attackButton(m_buttonTexture, {96.f, 416.f}, {0.8f, 0.8f}),  // must be changed again below
      m_useSkillButton(m_buttonTexture, {96.f, 500.f}, {0.8f, 0.8f}),
      m_useItemButton(m_buttonTexture, {96.f, 584.f}, {0.8f, 0.8f}),
      m_exitButton(m_buttonTexture, {96.f, 668.f}, {0.8f, 0.8f}),
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_boldFont(GetContext().GetResourceManager()->GetFont("main_bold_font")),
      m_navGrid(GetContext().GetWindow()->getSize().x, GetContext().GetWindow()->getSize().y, 51, 51),
      m_mobNavGrid(
          GetContext().GetWindow()->getSize().x, GetContext().GetWindow()->getSize().y, 51, 51),
      m_battleUnitInfo(context),
      m_questInfo(context, 250.0f, 150.0f),      // Quest info UI with reasonable size
      m_mobInfo(context, 250.0f, 160.0f),        // Mob info UI with reasonable size
      m_characterInfo(context, 250.0f, 160.0f),  // Character info UI with reasonable size
      m_chamberExitArea(GetContext().GetResourceManager()->GetTexture("empty_prop")),
      m_dungeon(dungeon),
      m_chamber(&dungeon.getChamber(0)),
      m_chamberExitPointer(GetContext().GetResourceManager()->GetTexture("pointer_prop"),
                           {1160.f, 540.f},
                           {4.f, 4.f}),
      m_bossHealthBar(120.f),
      m_pendingStateChange({StateAction::NONE}),
      m_cheatConsole(*GetContext().GetWindow())
{
    SetName("Dungeon State");

    ResourceManager* resources = GetContext().GetResourceManager();
    switch (dimension)
    {
        case DimensionType::LAVA:
            m_buttonTexture         = resources->GetTexture("lava_button");
            m_backgroundTexture     = resources->GetTexture("lava_base");
            m_bossBackgroundTexture = resources->GetTexture("lava_boss");
            break;
        case DimensionType::MISTY:
            m_buttonTexture         = resources->GetTexture("misty_button");
            m_backgroundTexture     = resources->GetTexture("misty_base");
            m_bossBackgroundTexture = resources->GetTexture("misty_boss");
            break;
        case DimensionType::CRYSTAL:
            m_buttonTexture         = resources->GetTexture("crystal_button");
            m_backgroundTexture     = resources->GetTexture("crystal_base");
            m_bossBackgroundTexture = resources->GetTexture("crystal_boss");
            break;
        case DimensionType::NORMAL:
            m_buttonTexture         = resources->GetTexture("normal_button");
            m_backgroundTexture     = resources->GetTexture("normal_base");
            m_bossBackgroundTexture = resources->GetTexture("normal_boss");
            break;
    }
    m_backgroundSprite.setTexture(m_backgroundTexture);
    m_attackButton   = Button(m_buttonTexture, {120.f, 416.f}, {.9f, .9f}),
    m_useSkillButton = Button(m_buttonTexture, {120.f, 500.f}, {.9f, .9f}),
    m_useItemButton  = Button(m_buttonTexture, {120.f, 584.f}, {.9f, .9f}),
    m_exitButton     = Button(m_buttonTexture, {120.f, 668.f}, {.9f, .9f}),

    m_chamberExitArea.setOrigin({0, 0});
    m_chamberExitArea.setScale({2.f, 25.f});
    m_chamberExitArea.setPosition({1200.f, 0.f});

    std::vector<std::vector<bool>> grid = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    m_navGrid.SetGrid(grid);
}

void DungeonState::Init()
{
    m_bossBacksound.setLooping(true);
    m_bossBacksound.setVolume(30);

    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    m_attackButton.setText("Attack", m_font, 32);
    m_attackButton.setHoverSound(m_buttonHoverSound);
    m_attackButton.setClickSound(m_buttonClickSound);

    m_useSkillButton.setText("Use Skill", m_font, 32);
    m_useSkillButton.setHoverSound(m_buttonHoverSound);
    m_useSkillButton.setClickSound(m_buttonClickSound);

    m_useItemButton.setText("Use Item", m_font, 32);
    m_useItemButton.setHoverSound(m_buttonHoverSound);
    m_useItemButton.setClickSound(m_buttonClickSound);

    m_exitButton.setText("Surrender", m_font, 32);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    m_exitButton.setOnClickCallback([this]() { m_showExitPopup = true; });

    m_useItemButton.setOnClickCallback([this]() { m_showPotionMenu = true; });

    m_useSkillButton.setOnClickCallback([this]() {
        Mob*         closestMob = getClosestUnitOfType<Mob>(m_mobsID, m_character->GetPosition());
        unsigned int targetId   = closestMob->GetId();
        if (closestMob == nullptr)
        {
            std::cerr << "No mobs available to attack!" << std::endl;
            return;
        }

        int                   initialHealth = closestMob->GetHealth();
        std::function<void()> callback      = [this, initialHealth, targetId] {
            m_triggerActionTurn = true;
            m_character->RefreshTurn();
            m_turnQueue.push(m_character->GetId());
            Unit* attackedMob = GetContext().GetUnitManager()->GetUnitOfType<Mob>(targetId);
            if (attackedMob && attackedMob->IsActive())
            {
                int damage = initialHealth - attackedMob->GetHealth();
                if (m_dungeon.getQuest().getType() == QuestType::DAMAGE)
                {
                    m_dungeon.updateDamageQuestProgress(damage);
                }
            }
            else
            {
                std::cerr << "Attacked mob not found!" << std::endl;
            }
        };

        std::function<void()> onKill = [this, targetId] {
            if (m_dungeon.getQuest().getType() == QuestType::KILL)
            {
                m_dungeon.updateKillQuestProgress(1);
            }
        };

        if (m_character->UseSkill(*closestMob, callback, onKill))
        {
            m_turnQueue.pop();
            m_isPlayerTurn = false;
        }
    });
    m_attackButton.setOnClickCallback([this]() {
        Mob*         closestMob = getClosestUnitOfType<Mob>(m_mobsID, m_character->GetPosition());
        unsigned int targetId   = closestMob->GetId();
        if (closestMob == nullptr)
        {
            std::cerr << "No mobs available to attack!" << std::endl;
            return;
        }
        int initialHealth = closestMob->GetHealth();

        // set callback to trigger
        m_turnQueue.pop();
        m_isPlayerTurn                 = false;
        std::function<void()> callback = [this, initialHealth, targetId] {
            m_character->RefreshTurn();
            m_triggerActionTurn = true;
            m_turnQueue.push(m_character->GetId());
            Unit* attackedMob = GetContext().GetUnitManager()->GetUnitOfType<Mob>(targetId);
            if (attackedMob && attackedMob->IsActive())
            {
                int damage = initialHealth - attackedMob->GetHealth();
                if (m_dungeon.getQuest().getType() == QuestType::DAMAGE)
                {
                    m_dungeon.updateDamageQuestProgress(damage);
                }
            }
            else
            {
                std::cerr << "Attacked mob not found!" << std::endl;
            }
        };

        std::function<void()> onKill = [this, initialHealth, targetId] {
            Mob* attackedMob = GetContext().GetUnitManager()->GetUnitOfType<Mob>(targetId);
            if (attackedMob)
            {
                m_mobsID.erase(std::remove(m_mobsID.begin(), m_mobsID.end(), targetId),
                               m_mobsID.end());

                if (m_dungeon.getQuest().getType() == QuestType::DAMAGE)
                {
                    m_dungeon.updateDamageQuestProgress(initialHealth);
                }
            }

            // Update the quest progress when mobs are cleared
            if (m_dungeon.getQuest().getType() == QuestType::KILL)
            {
                m_dungeon.updateKillQuestProgress(1);
            }
        };
        m_character->Attack(*closestMob, callback, onKill);
    });

    m_mobsID = m_chamber->getMobsId();

    std::vector<sf::Vector2f> spawnPoints = generateMobSpawnPoints(
        {windowSize.x / 2.0f + 400.f * windowSize.x / 1820.f, 780.0f * windowSize.y / 1024.f},
        m_mobsID.size(),
        true);
    for (int i = 0; i < m_mobsID.size(); ++i)
    {
        AnimatedUnit* mob = GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(m_mobsID[i]);
        mob->SetScale(8.f, 8.f);
        mob->SetActive(true);
        mob->SetPosition(spawnPoints[i]);
        mob->SetControlledByPlayer(false);
        mob->SetNavGrid(m_mobNavGrid);
        mob->SetShowUI(true);
        mob->SetDirection(Direction::WEST);
    }

    m_character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    if (m_character)
    {
        m_character->SetScale({8.0f, 8.0f});
        m_character->SetPosition(
            {windowSize.x / 2.0f - 400.f * windowSize.x / 1820.f, 780.0f * windowSize.y / 1024.f});
        m_character->SetDirection(Direction::EAST);
        m_character->SetShowUI(true);
        m_character->SetControlledByPlayer(false);

        m_character->SetNavGrid(m_navGrid);
        m_character->SetMoveSpeed(m_character->GetMoveSpeed() * 1.5f);
        m_battleUnitInfo.updateSpriteTexture(*m_character);
    }
    else
    {
        std::cerr << "Character not found!" << std::endl;
        return;
    }

    m_turnQueue.push(m_character->GetId());
    for (auto id : m_mobsID)
    {
        m_turnQueue.push(id);
    }

    m_triggerActionTurn = false;

    initializeCheat();
}

State::StateChange DungeonState::ProcessEvent(const sf::Event& event)
{
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        StateChange change   = std::move(m_pendingStateChange);
        m_pendingStateChange = {StateAction::NONE};
        return change;
    }

    m_cheatConsole.processEvent(event);

    return StateChange {};
}

State::StateChange DungeonState::Update(const sf::Time& dt)
{
    m_cheatConsole.update(dt.asSeconds());

    // checks if player die
    if (!m_walkToExit && m_turnQueue.front() == m_character->GetId() && !m_character->IsActive() &&
        !m_isTransitioning)
    {
        if (m_dungeon.getRank() == Dungeon::RANK_SPECIAL)
        {
            vector<Item> loot = m_dungeon.getLoot();
            for (auto& item : loot)
            {
                try
                {
                    GetContext().GetBackpack()->addItem(item, 1);
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    std::cerr << "Backpack is full!" << std::endl;
                }
            }
        }
        else
        {}
        Necromancer* necromancer =
            GetContext().GetUnitManager()->GetUnitOfType<Necromancer>(GetContext().GetCharacterId());
        if (necromancer)
        {
            m_isTransitioning              = true;
            std::function<void()> callback = [this]() {
                m_isTransitioning = false;
                return StateChange {StateAction::POP};
            };
            necromancer->KillSummons(callback);
        }
        else
        {
            m_isTransitioning = false;
        }
        int expPenalty  = 0;
        int goldPenalty = 0;
        m_dungeon.getPenalty(expPenalty, goldPenalty);
        Character* character =
            GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
        expPenalty  = std::min(expPenalty, character->GetExp());
        goldPenalty = std::min(goldPenalty, character->GetGold());
        character->AddExp(-expPenalty);
        character->AddGold(-goldPenalty);
        return StateChange {StateAction::POP};
    }

    if (!m_walkToExit && m_mobsID.size() == 0 && m_turnQueue.front() == m_character->GetId() &&
        !m_isTransitioning)
    {
        m_walkToExit = true;
        m_character->SetControlledByPlayer(true);
        std::cout << "MOB CLEARED" << std::endl;

        m_dungeon.clearChamber(m_chamber->getChamberNumber() - 1);
        Character* character =
            GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
        character->AddExp(m_chamber->getExpReward());
        character->AddGold(m_chamber->getGoldReward());

        Necromancer* necromancer =
            GetContext().GetUnitManager()->GetUnitOfType<Necromancer>(GetContext().GetCharacterId());
        // checks if player finish dungeon
        if (m_dungeon.areAllChambersCleared())
        {
            vector<Item> loot = m_dungeon.getLoot();
            for (auto& item : loot)
            {
                try
                {
                    GetContext().GetBackpack()->addItem(item, 1);
                    std::cout << "Loot added to backpack: " << item.getName() << std::endl;
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    std::cerr << "Backpack is full!" << std::endl;
                }
            }

            vector<Item> mobloots = m_dungeon.getMobLoot();
            for (auto& mobloot : mobloots)
            {
                try
                {
                    GetContext().GetBackpack()->addItem(mobloot, 1);
                    std::cout << "Loot added to backpack: " << mobloot.getName() << std::endl;
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    std::cerr << "Backpack is full!" << std::endl;
                }
            }

            if (necromancer)
            {
                m_isTransitioning              = true;
                std::function<void()> callback = [this]() {
                    m_isTransitioning = false;
                    return StateChange {StateAction::POP};
                };
                necromancer->KillSummons(callback);
            }

            // If quest is completed, add quest rewards to player
            if (m_dungeon.getQuest().isValid() && m_dungeon.isQuestCompleted())
                if (m_dungeon.getQuest().isValid() && m_dungeon.isQuestCompleted())
                {
                    Character* character = GetContext().GetUnitManager()->GetUnitOfType<Character>(
                        GetContext().GetCharacterId());
                    character->AddExp(m_dungeon.getQuestExpReward());
                    character->AddGold(m_dungeon.getQuestGoldReward());

                    // Add quest item reward to backpack if there is one
                    Item questItem = m_dungeon.getQuestRewardItem();
                    if (!questItem.getItemID().empty())
                    {
                        try
                        {
                            GetContext().GetBackpack()->addItem(questItem, 1);
                            cout << "Quest item added to backpack: " << questItem.getName() << endl;
                        }
                        catch (const std::exception& e)
                        {
                            std::cerr << "Exception: " << e.what() << std::endl;
                            std::cerr << "Backpack is full, couldn't add quest reward item!"
                                      << std::endl;
                        }
                    }
                }

            return StateChange {StateAction::POP};
        }
        if (necromancer)
        {
            m_isTransitioning              = true;
            std::function<void()> callback = [this]() { m_isTransitioning = false; };
            necromancer->KillSummons(callback);
        }
    }

    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    m_exitButton.setActive(m_isPlayerTurn);
    m_attackButton.setActive(m_isPlayerTurn);
    m_useSkillButton.setActive(m_isPlayerTurn);
    m_useItemButton.setActive(m_isPlayerTurn);

    if (!m_walkToExit && m_triggerActionTurn && !m_isPlayerTurn)
    {
        std::cout << "Mob turn triggered." << std::endl;
        m_triggerActionTurn = false;
        playMobTurn();
    }

    bool isInChamberExit = false;
    if (character)
    {
        auto          pos           = character->GetPosition();
        sf::FloatRect chamberBounds = m_chamberExitArea.getGlobalBounds();
        if (chamberBounds.contains(pos))
        {
            isInChamberExit = true;
        }
    }

    if (isInChamberExit && !m_wasInChamberExit)
    {
        sf::Vector2u windowSize = GetContext().GetWindow()->getSize();
        character->SetPosition(
            {windowSize.x / 2.0f - 400.f * windowSize.x / 1820.f, 780.0f * windowSize.y / 1024.f});
        character->SetControlledByPlayer(false);
        m_walkToExit = false;
        nextChamber();
    }

    m_wasInChamberExit = isInChamberExit;

    if (!(m_showExitPopup || m_walkToExit))
    {
        m_attackButton.update(*GetContext().GetWindow());
        m_useSkillButton.update(*GetContext().GetWindow());
        m_useItemButton.update(*GetContext().GetWindow());
        m_exitButton.update(*GetContext().GetWindow());
    }

    if (m_walkToExit)
    {
        m_chamberExitPointer.Update(dt);
    }

    // handle any pending state-change (from your buttons, etc)
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
        return std::move(m_pendingStateChange);

    return StateChange {StateAction::NONE};
}

void DungeonState::Draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(196, 196, 196));

    window.draw(m_backgroundSprite);

    window.draw(m_chamberExitArea);
    if (!m_walkToExit)
    {
        m_attackButton.draw(window);
        m_useSkillButton.draw(window);
        m_useItemButton.draw(window);
        m_exitButton.draw(window);
    }
    else
    {
        m_chamberExitPointer.Draw(window);
    }
}

void DungeonState::RenderUI()
{
    m_cheatConsole.render();

    // Render character info - ADDED THIS SECTION
    Character* character =
        GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
    if (character && m_character)
    {
        m_characterInfo.render(*character, *m_character);
    }

    bool isBossRoom = m_chamber->getIsBossRoom();

    // Render boss health bar if in boss room and there are mobs
    if (isBossRoom && m_mobsID.size() != 0)
    {
        Unit* boss = GetContext().GetUnitManager()->GetUnitOfType<Unit>(m_mobsID[0]);
        m_bossHealthBar.render(*boss);
    }

    // Render quest info
    m_questInfo.render(m_dungeon);

    // Render mob info if there are mobs, not in a boss room, and not in the walk to exit state
    if (!m_mobsID.empty() && !m_walkToExit)
    {
        m_mobInfo.render(m_mobsID, isBossRoom);
    }

    // Render potion menu if flag is set
    if (m_showPotionMenu)
    {
        renderPotion();
    }

    // Display exit confirmation popup
    if (m_showExitPopup)
    {
        // Center the modal window
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(320, 160));

        ImGui::OpenPopup("Surrender Confirmation");
        m_showExitPopup = false;  // prevent reopening every frame
    }

    if (ImGui::BeginPopupModal("Surrender Confirmation",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav |
                                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Apply styling directly inside the modal
        const float sectionSpacing = 3.0f;

        // Header
        ImGui::SetWindowFontScale(1.3f);
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Surrender Dungeon");
        ImGui::SetWindowFontScale(1.0f);

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Confirmation message
        ImGui::TextWrapped("Do you really want to surrender? You will lose EXP and Gold!");

        ImGui::Dummy(ImVec2(0, sectionSpacing + 1.0f));

        // Buttons at the bottom
        float contentWidth = ImGui::GetContentRegionAvail().x;
        float buttonWidth  = 120.0f;
        float startPos     = (contentWidth - (buttonWidth * 2)) / 2.0f + 12.0f;

        ImGui::SetCursorPosX(startPos);

        // Yes button (red for warning)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

        if (ImGui::Button("Yes", ImVec2(buttonWidth, 40)))
        {
            int expPenalty  = 0;
            int goldPenalty = 0;
            m_dungeon.getPenalty(expPenalty, goldPenalty);
            Character* character = GetContext().GetUnitManager()->GetUnitOfType<Character>(
                GetContext().GetCharacterId());
            expPenalty  = std::min(expPenalty, character->GetExp());
            goldPenalty = std::min(goldPenalty, character->GetGold());
            character->AddExp(-expPenalty);
            character->AddGold(-goldPenalty);
            Necromancer* necromancer = GetContext().GetUnitManager()->GetUnitOfType<Necromancer>(
                GetContext().GetCharacterId());
            if (necromancer)
            {
                m_isTransitioning              = true;
                std::function<void()> callback = [this]() {
                    m_isTransitioning = false;
                    ImGui::CloseCurrentPopup();
                    m_pendingStateChange = StateChange {StateAction::POP};
                };
                necromancer->KillSummons(callback);
            }
            else
            {
                ImGui::CloseCurrentPopup();
                m_pendingStateChange = StateChange {StateAction::POP};
            }
        }

        // Pop the yes button styles
        ImGui::PopStyleColor(4);

        ImGui::SameLine();

        // No button (green for continue)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

        if (ImGui::Button("No", ImVec2(buttonWidth, 40)))
        {
            ImGui::CloseCurrentPopup();
        }

        // Pop the no button styles
        ImGui::PopStyleColor(4);

        ImGui::EndPopup();
    }
}

void DungeonState::renderPotion()
{
    // Filter potions from backpack
    std::vector<std::pair<Item, int>*> potionItems;

    std::vector<std::vector<std::pair<Item, int>>> backpack =
        GetContext().GetBackpack()->getBackpackData();

    // Iterate through backpack to find potions
    for (int i = 0; i < backpack.size(); ++i)
    {
        for (int j = 0; j < backpack[i].size(); ++j)
        {
            Item& item = backpack[i][j].first;
            if (item.getType() == "Potion")
            {
                potionItems.push_back(&backpack[i][j]);
            }
        }
    }

    // Debug all of all backpack items
    std::cout << "Backpack items:" << std::endl;
    for (const auto& backpackRow : backpack)
    {
        for (const auto& itemPair : backpackRow)
        {
            std::cout << "Item: " << itemPair.first.getName() << ", Quantity: " << itemPair.second
                      << std::endl;
        }
    }

    // Consolidate identical potions
    std::map<std::string, std::pair<Item, int>> consolidatedPotions;
    for (auto& pair : potionItems)
    {
        if (pair)
        {  // Check if the pointer is valid
            const Item& item     = pair->first;
            int         quantity = pair->second;

            if (!item.isNull())
            {  // Make sure item is valid
                // If item already exists, add quantity
                if (consolidatedPotions.find(item.getItemID()) != consolidatedPotions.end())
                {
                    consolidatedPotions[item.getItemID()].second += quantity;
                }
                else
                {
                    consolidatedPotions[item.getItemID()] = {item, quantity};
                }
            }
        }
    }

    // Convert to vector for grid display
    std::vector<std::pair<Item, int>> potionList;
    for (auto& [_, pair] : consolidatedPotions)
    {
        potionList.push_back(pair);
    }

    // Create semi-transparent overlay background
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));  // Semi-transparent
                                                                               // black
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("PotionOverlay",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Define grid dimensions
    static constexpr int SLOT_SIZE  = 138;
    static constexpr int GRID_WIDTH = 4;
    int GRID_HEIGHT = (potionList.size() + GRID_WIDTH - 1) / GRID_WIDTH;  // Ceiling division
    if (GRID_HEIGHT == 0)
        GRID_HEIGHT = 1;  // Ensure at least 1 row

    // Position the grid window in the center of the screen
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImVec2 gridSize    = ImVec2(GRID_WIDTH * SLOT_SIZE + 13, std::min(4, GRID_HEIGHT) * SLOT_SIZE);
    ImVec2 windowPos   = ImVec2((displaySize.x - gridSize.x) * 0.5f,
                              (displaySize.y - gridSize.y) * 0.5f - 65);  // Centered, adjusted up
                                                                            // for description box

    // Close button at the top right of the overlay
    ImVec2 closeButtonPos = ImVec2(displaySize.x - 50, 10);
    ImGui::SetCursorPos(closeButtonPos);
    if (ImGui::Button("X", ImVec2(40, 40)))
    {
        m_showPotionMenu = false;  // Set the flag to close the menu
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        return;
    }

    // Position the grid window
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(gridSize);

    // Style for grid window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    // Colors to match brownish background
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // Brownish
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.55f, 0.30f, 0.20f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.65f, 0.40f, 0.25f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.70f, 0.45f, 0.30f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    ImGui::Begin("PotionGrid",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // Track hovered item for description
    static Item hoveredItem;
    bool        hasHoveredItem = false;

    // Draw the grid
    int idx = 0;
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            // Calculate position
            float slotX = x * SLOT_SIZE;
            float slotY = y * SLOT_SIZE;

            // Set cursor position
            ImGui::SetCursorPos(ImVec2(slotX, slotY));

            // Generate unique ID
            ImGui::PushID(y * GRID_WIDTH + x);

            Item item;
            int  quantity    = 0;
            bool isEmptySlot = true;

            // Get item if available
            if (idx < potionList.size())
            {
                item        = potionList[idx].first;
                quantity    = potionList[idx].second;
                isEmptySlot = false;
                idx++;
            }

            // Apply styling
            if (isEmptySlot)
            {
                // For empty slots - no hover/click effect
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                      ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // No hover effect
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                      ImVec4(0.35f, 0.18f, 0.12f, 1.0f));  // No click effect
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));
            }
            else
            {
                // For item slots - normal hover/click effects
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.35f, 0.18f, 0.12f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.25f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.55f, 0.35f, 0.25f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

            // Draw button slot
            bool clicked = ImGui::Button("", ImVec2(SLOT_SIZE - 2, SLOT_SIZE - 2));

            // Check for hover - only for non-empty slots
            if (ImGui::IsItemHovered() && !isEmptySlot)
            {
                hoveredItem    = item;
                hasHoveredItem = true;
            }

            // Handle click - only for non-empty slots
            if (clicked && !isEmptySlot)
            {
                // Get Character
                Character* character = GetContext().GetUnitManager()->GetUnitOfType<Character>(
                    GetContext().GetCharacterId());
                if (character)
                {
                    try
                    {
                        // First remove item from backpack
                        Item usedItem = GetContext().GetBackpack()->takeItem(item, 1);
                        std::cout << "Using potion: " << usedItem.getName() << std::endl;

                        // Apply each effect from the potion
                        const std::vector<std::string>& effects = item.getEffects();
                        for (const std::string& effectName : effects)
                        {
                            try
                            {
                                std::cout << "Applying effect: " << effectName << std::endl;
                                auto effect = EffectFactory::CreateUnique(effectName);
                                character->AddEffect(std::move(effect));
                            }
                            catch (const std::exception& e)
                            {
                                std::cerr << "Failed to apply effect " << effectName << ": "
                                          << e.what() << std::endl;
                            }
                        }

                        // Don't close the potion grid, just refresh the display
                        // We need to recalculate the list after using an item
                        potionItems = GetContext().GetBackpack()->filterItemsByType("Potion");

                        // Update consolidated potions
                        consolidatedPotions.clear();
                        for (auto& pair : potionItems)
                        {
                            if (pair)
                            {  // Check if the pointer is valid
                                const Item& updatedItem     = pair->first;
                                int         updatedQuantity = pair->second;

                                if (!updatedItem.isNull())
                                {  // Make sure item is valid
                                    if (consolidatedPotions.find(updatedItem.getItemID()) !=
                                        consolidatedPotions.end())
                                    {
                                        consolidatedPotions[updatedItem.getItemID()].second +=
                                            updatedQuantity;
                                    }
                                    else
                                    {
                                        consolidatedPotions[updatedItem.getItemID()] = {
                                            updatedItem, updatedQuantity};
                                    }
                                }
                            }
                        }

                        // Rebuild the potion list
                        potionList.clear();
                        for (auto& [_, pair] : consolidatedPotions)
                        {
                            potionList.push_back(pair);
                        }

                        // If no more potions, close the grid
                        if (potionList.empty())
                        {
                            m_showPotionMenu = false;  // Set flag to close the menu
                            ImGui::End();              // End grid
                            ImGui::End();              // End overlay
                            ImGui::PopStyleVar(6);
                            ImGui::PopStyleColor(7);

                            // Update turn (using an item takes the player's turn)
                            m_triggerActionTurn = true;
                            m_character->RefreshTurn();
                            m_turnQueue.push(m_character->GetId());
                            m_turnQueue.pop();
                            m_isPlayerTurn = false;
                            return;
                        }

                        Stats stats = character->GetStats();
                        std::cout << "Strength: " << stats.strength << std::endl;
                        std::cout << "Agility: " << stats.agility << std::endl;
                        std::cout << "Intelligence: " << stats.intelligence << std::endl;
                        std::cout << "Buff Multiplier: " << stats.buffMultiplier << std::endl;
                        std::cout << "Critical Strike Multiplier: " << stats.criticalStrikeMultiplier
                                  << std::endl;
                        std::cout << "Critical Strike Chance: " << stats.criticalStrikeChance
                                  << std::endl;
                        std::cout << "Skip Turn Chance: " << stats.skipTurnChance << std::endl;
                        std::cout << "Luck: " << stats.luck << std::endl;
                        std::cout << "Physical Defense: " << stats.physicalDefense << std::endl;
                        std::cout << "Magic Defense: " << stats.magicDefense << std::endl;
                        std::cout << "Dodge Chance: " << stats.dodgeChance << std::endl;
                        std::cout << "Accuracy: " << stats.accuracy << std::endl;
                        std::cout << "Status Resistance: " << stats.statusResistance << std::endl;
                        std::cout << "Haste Multiplier: " << stats.hasteMultiplier << std::endl;
                        std::cout << "Resource Cost Multiplier: " << stats.resourceCostMul
                                  << std::endl;

                        // Update turn (using an item takes the player's turn)
                        m_triggerActionTurn = true;
                        m_character->RefreshTurn();
                        m_turnQueue.push(m_character->GetId());
                        m_turnQueue.pop();
                        m_isPlayerTurn   = false;
                        m_showPotionMenu = false;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Failed to use potion: " << e.what() << std::endl;
                    }
                }
            }

            // Get the position where we drew the button
            ImVec2 buttonPos = ImGui::GetItemRectMin();
            ImVec2 buttonCenter =
                ImVec2(buttonPos.x + (SLOT_SIZE - 2) / 2.0f, buttonPos.y + (SLOT_SIZE - 2) / 2.0f);

            // Render item texture or empty slot texture
            if (isEmptySlot)
            {
                // Try to render empty slot texture if available
                try
                {
                    sf::Texture* emptyTexture = &GetContext().GetResourceManager()->GetTexture("NO"
                                                                                               "L");
                    if (emptyTexture)
                    {
                        ImTextureID textureId =
                            (ImTextureID)(intptr_t)emptyTexture->getNativeHandle();
                        ImVec2 imageSize(SLOT_SIZE - 16, SLOT_SIZE - 16);
                        ImVec2 imagePos(buttonCenter.x - imageSize.x / 2.0f,
                                        buttonCenter.y - imageSize.y / 2.0f);
                        ImGui::SetCursorScreenPos(imagePos);
                        ImGui::Image(textureId, imageSize);
                    }
                }
                catch (const std::exception& e)
                {
                    // If texture not found, just skip rendering the empty texture
                    std::cerr << "Empty slot texture not found: " << e.what() << std::endl;
                }
            }
            else
            {
                // Render item texture
                try
                {
                    sf::Texture* texture =
                        &GetContext().GetResourceManager()->GetTexture(item.getItemID());
                    if (texture)
                    {
                        ImTextureID textureId = (ImTextureID)(intptr_t)texture->getNativeHandle();
                        ImVec2      imageSize(SLOT_SIZE - 16, SLOT_SIZE - 16);
                        ImVec2      imagePos(buttonCenter.x - imageSize.x / 2.0f,
                                        buttonCenter.y - imageSize.y / 2.0f);
                        ImGui::SetCursorScreenPos(imagePos);
                        ImGui::Image(textureId, imageSize);

                        // Increase font size for item details
                        ImGui::SetWindowFontScale(1.1f);

                        // Rarity label
                        char r = item.getRarity();
                        ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 12, buttonPos.y + 10));
                        ImVec4 rarityColor = Color::GetRarityColor(r);
                        ImGui::PushStyleColor(ImGuiCol_Text, rarityColor);
                        ImGui::Text("%c", r);
                        ImGui::PopStyleColor();

                        // Quantity label
                        ImGui::SetCursorScreenPos(
                            ImVec2(buttonPos.x + SLOT_SIZE - 30, buttonPos.y + SLOT_SIZE - 26));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.75f, 0.8f, 1.0f));
                        ImGui::Text("%d", quantity);
                        ImGui::PopStyleColor();

                        // Reset font scale
                        ImGui::SetWindowFontScale(1.0f);
                    }
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Failed to load texture for item " << item.getItemID() << ": "
                              << e.what() << std::endl;
                }
            }

            // Pop style and ID
            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(4);
            ImGui::PopID();
        }
    }

    ImGui::End();  // End PotionGrid

    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(6);

    // Force exact content height
    ImGui::SetCursorPos(ImVec2(0, GRID_HEIGHT * SLOT_SIZE - 1));
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
    ImGui::Button("##endspacer", ImVec2(SLOT_SIZE, 1));
    ImGui::PopStyleVar();

    // Render description box below the grid
    if (hasHoveredItem && !hoveredItem.isNull())
    {
        // Potion description box
        ImVec2 descPos = ImVec2(windowPos.x, windowPos.y + gridSize.y + 10);
        ImGui::SetNextWindowPos(descPos);
        ImGui::SetNextWindowSize(ImVec2(gridSize.x, 75.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25f, 0.15f, 0.1f, 0.9f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.42f, 0.22f, 0.14f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

        ImGui::Begin("PotionDescription",
                     nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // Item name
        std::string itemName = hoveredItem.getName();
        std::replace(itemName.begin(), itemName.end(), '_', ' ');

        // Set color based on item type
        ImVec4 itemColor = Color::GetTypeColor(hoveredItem.getType());
        ImGui::PushStyleColor(ImGuiCol_Text, itemColor);
        ImGui::Text("%s", itemName.c_str());
        ImGui::PopStyleColor();

        // Effects
        std::vector<std::string> effects = hoveredItem.getEffects();
        std::string              effectText;
        for (size_t i = 0; i < effects.size(); ++i)
        {
            if (i > 0)
                effectText += ", ";
            effectText += effects[i];
        }

        if (!effectText.empty())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("Effects: %s", effectText.c_str());
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::Text("Effects: None");
        }

        // Description
        ImGui::TextWrapped("%s", hoveredItem.getDescription().c_str());

        ImGui::End();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);
    }

    ImGui::End();  // End overlay
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void DungeonState::Pause()
{
    if (m_chamber->getIsBossRoom())
    {
        m_bossBacksound.stop();
    }
}

void DungeonState::Resume()
{
    if (m_chamber->getIsBossRoom())
    {
        m_bossBacksound.play();
    }
}

void DungeonState::Exit()
{
    if (m_chamber->getIsBossRoom())
    {
        m_bossBacksound.stop();
    }
    for (auto id : m_mobsID)
    {
        GetContext().GetUnitManager()->RemoveUnit(id);
    }
    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    if (character)
    {
        character->SetMoveSpeed(character->GetMoveSpeed() / 1.5f);
    }

    // Save everything related to the user loaded thing
}

void DungeonState::playMobTurn()
{
    unsigned int mobId = m_turnQueue.front();

    // if mobId not in m_mobsID, pop and return
    if (std::find(m_mobsID.begin(), m_mobsID.end(), mobId) == m_mobsID.end())
    {
        m_turnQueue.pop();
        m_triggerActionTurn = true;
        return;
    }

    AnimatedUnit* mob =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(m_turnQueue.front());
    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    // NOTE: potential for refactoring later (if not close to deadline, but this works fine)
    std::vector<Summon*>      summons = GetContext().GetUnitManager()->GetAllUnitsOfType<Summon>();
    std::vector<unsigned int> summonIDs;
    for (auto summon : summons)
    {
        summonIDs.push_back(summon->GetId());
    }

    // set callback
    std::function<void()> callback = [this, mob]() {
        std::cout << "Mob " << mob->GetName() << " turn finished." << std::endl;
        m_triggerActionTurn = true;
        m_turnQueue.pop();
        m_turnQueue.push(mob->GetId());
        if (m_turnQueue.front() == m_character->GetId())
        {
            m_isPlayerTurn      = true;
            m_triggerActionTurn = false;
        }
    };

    AnimatedUnit* summon = getClosestUnitOfType<AnimatedUnit>(summonIDs, mob->GetPosition());
    if (summon == nullptr)
    {
        mob->Attack(*character, callback);
        return;
    }
    else
    {
        Wildfire* wildfire = GetContext().GetUnitManager()->GetUnitOfType<Wildfire>(summon->GetId());
        if (wildfire)
        {
            mob->Attack(*character, callback);
            return;
        }
        // compare distance to character and summon and choose the closest
        float mobDist = std::sqrt(std::pow(mob->GetPosition().x - character->GetPosition().x, 2) +
                                  std::pow(mob->GetPosition().y - character->GetPosition().y, 2));
        float summonDist = std::sqrt(std::pow(mob->GetPosition().x - summon->GetPosition().x, 2) +
                                     std::pow(mob->GetPosition().y - summon->GetPosition().y, 2));
        if (mobDist < summonDist)
        {
            mob->Attack(*character, callback);
        }
        else
        {
            mob->Attack(*summon, callback);
        }
    }

    return;
}

void DungeonState::nextChamber()
{
    // remove all mobs from unit manager
    for (auto id : m_mobsID)
    {
        GetContext().GetUnitManager()->RemoveUnit(id);
    }

    m_chamber = &m_dungeon.getChamber(m_chamber->getChamberNumber());
    m_mobsID  = m_chamber->getMobsId();

    if (m_chamber->getIsBossRoom())
    {
        m_bossBacksound.play();
        m_backgroundSprite.setTexture(m_bossBackgroundTexture);
    }
    else
    {
        m_backgroundSprite.setTexture(m_backgroundTexture);
    }

    // spawn new mobs
    std::vector<sf::Vector2f> spawnPoints =
        generateMobSpawnPoints({GetContext().GetWindow()->getSize().x / 2.0f +
                                    400.f * GetContext().GetWindow()->getSize().x / 1820.f,
                                780.0f * GetContext().GetWindow()->getSize().y / 1024.f},
                               m_mobsID.size(),
                               true);
    for (int i = 0; i < m_mobsID.size(); ++i)
    {
        AnimatedUnit* mob = GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(m_mobsID[i]);
        mob->SetScale(8.f, 8.f);
        mob->SetActive(true);
        mob->SetPosition(spawnPoints[i]);
        mob->SetControlledByPlayer(false);
        mob->SetNavGrid(m_mobNavGrid);
        mob->SetShowUI(true);
        mob->SetDirection(Direction::WEST);

        m_turnQueue.push(m_mobsID[i]);
    }
    m_triggerActionTurn = false;
    m_isPlayerTurn      = true;
}

std::vector<sf::Vector2f> DungeonState::generateMobSpawnPoints(const sf::Vector2f& center,
                                                               unsigned int        mobCount,
                                                               bool                isLeftHalf)
{
    float                     radius = 80.f;
    std::vector<sf::Vector2f> spawnPoints;
    spawnPoints.reserve(mobCount);

    // single mob goes in the center
    if (mobCount == 1)
    {
        spawnPoints.emplace_back(center);
        return spawnPoints;
    }

    // define start angle based on which half-circle:
    // Right  half: from -90° to +90°  (i.e. -π/2 … +π/2)
    // Left   half: from +90° to +270° (i.e.  π/2 … 3π/2)
    float startAngle = (isLeftHalf) ? M_PI_2 : -M_PI_2;
    float arc        = M_PI;  // 180° sweep

    // divide that arc into (mobCount–1) segments so endpoints line up
    float step = arc / static_cast<float>(mobCount - 1);

    for (unsigned int i = 0; i < mobCount; ++i)
    {
        float angle = startAngle + step * static_cast<float>(i);
        float x     = center.x + std::cos(angle) * radius;
        float y     = center.y + std::sin(angle) * radius;
        spawnPoints.emplace_back(x, y);
    }

    return spawnPoints;
}

void DungeonState::initializeCheat()
{
    // Cheat console setup
    m_cheatConsole.registerCommand(
        "add_gold",
        [this](const std::vector<std::string>& args) -> std::string {
            int amount = 1000;
            if (!args.empty())
            {
                try
                {
                    amount = std::stoi(args[0]);
                }
                catch (...)
                {
                    return "Invalid amount";
                }
            }
            if (m_character)
            {
                Character* character =
                    GetContext().GetUnitManager()->GetUnitOfType<Character>(m_character->GetId());
                if (character)
                {
                    character->AddGold(amount);
                    return "Added $" + std::to_string(amount) + ". New balance: $" +
                           std::to_string(character->GetGold());
                }
                else
                {
                    return "Character not found";
                }
            }
            return "Character not initialized";
        },
        "Add gold to the character");

    m_cheatConsole.registerCommand(
        "add_exp",
        [this](const std::vector<std::string>& args) -> std::string {
            int amount = 1000;
            if (!args.empty())
            {
                try
                {
                    amount = std::stoi(args[0]);
                }
                catch (...)
                {
                    return "Invalid amount";
                }
            }
            if (m_character)
            {
                Character* character =
                    GetContext().GetUnitManager()->GetUnitOfType<Character>(m_character->GetId());
                if (character)
                {
                    character->AddExp(amount);
                    return "Added " + std::to_string(amount) +
                           " EXP. New EXP: " + std::to_string(character->GetExp());
                }
                else
                {
                    return "Character not found";
                }
            }
            return "Character not initialized";
        },
        "Add experience points to the character");

    // Reset effects
    m_cheatConsole.registerCommand(
        "reset",
        [this](const std::vector<std::string>& args) -> std::string {
            if (m_character)
            {
                Character* character =
                    GetContext().GetUnitManager()->GetUnitOfType<Character>(m_character->GetId());
                if (character)
                {
                    character->Reset();
                    return "Character reset";
                }
                else
                {
                    return "Character not found";
                }
            }
            return "Character not initialized";
        },
        "Reset all effects and restore hp and mp on the character");

    // God mode
    m_cheatConsole.registerCommand(
        "god_mode",
        [this](const std::vector<std::string>& args) -> std::string {
            if (m_character)
            {
                Character* character =
                    GetContext().GetUnitManager()->GetUnitOfType<Character>(m_character->GetId());
                if (character)
                {
                    if (args.size() > 0)
                    {
                        if (args[0] == "on")
                        {
                            Stats temp       = character->GetStats();
                            temp.dodgeChance = 1.0f;
                            character->SetStats(temp);
                            character->SetAttackDamage(character->GetAttackDamage() *
                                                       10000.0f);  // set attack damage to
                                                                   // 10000x
                            return "God mode enabled";
                        }
                        else if (args[0] == "off")
                        {
                            Stats temp       = character->GetStats();
                            temp.dodgeChance = 0.0f;
                            character->SetStats(temp);
                            character->SetAttackDamage(character->GetAttackDamage() /
                                                       10000.0f);  // set attack damage to
                                                                   // normal
                            return "God mode disabled";
                        }
                        else
                        {
                            return "Invalid argument. Use 'on' or 'off'";
                        }
                    }
                    else
                    {
                        return "Invalid argument. Use 'on' or 'off'";
                    }
                }
                else
                {
                    return "Character not found";
                }
            }
            return "Character not initialized";
        },
        "Toggle god mode on the character");

    // finish chamber
    m_cheatConsole.registerCommand(
        "finish_chamber",
        [this](const std::vector<std::string>& args) -> std::string {
            if (m_chamber)
            {
                for (auto id : m_mobsID)
                {
                    GetContext().GetUnitManager()->RemoveUnit(id);
                }
                m_mobsID.clear();
                while (m_turnQueue.size() > 0)
                {
                    m_turnQueue.pop();
                }
                m_turnQueue.push(m_character->GetId());
                return "Chamber finished";
            }
            else
            {
                return "Chamber not found";
            }
        },
        "Finish the current chamber");

    // finish dungeon
    m_cheatConsole.registerCommand(
        "finish_dungeon",
        [this](const std::vector<std::string>& args) -> std::string {
            // clear mobs
            for (auto id : m_mobsID)
            {
                GetContext().GetUnitManager()->RemoveUnit(id);
            }
            m_mobsID.clear();
            // clear stack except for character
            while (m_turnQueue.size() > 0)
            {
                m_turnQueue.pop();
            }
            m_turnQueue.push(m_character->GetId());

            for (int i = 0; i < (int)m_dungeon.getChambers().size(); ++i)
            {
                if (!m_dungeon.getChamber(i).getIsCleared())
                {
                    Character* character = GetContext().GetUnitManager()->GetUnitOfType<Character>(
                        GetContext().GetCharacterId());
                    if (character)
                    {
                        character->AddExp(m_dungeon.getChamber(i).getExpReward());
                        character->AddGold(m_dungeon.getChamber(i).getGoldReward());
                    }

                    m_dungeon.clearChamber(i);
                }
            }
            m_dungeon.setCleared(true);
            return "Dungeon finished";
        },
        "Finish the current dungeon");

    // finish quest
    m_cheatConsole.registerCommand(
        "finish_quest",
        [this](const std::vector<std::string>& args) -> std::string {
            if (m_dungeon.getQuest().isValid())
            {
                if (m_dungeon.getQuest().getType() == QuestType::KILL)
                {
                    m_dungeon.updateKillQuestProgress(m_dungeon.getQuest().getTargetCount());
                }
                else if (m_dungeon.getQuest().getType() == QuestType::DAMAGE)
                {
                    m_dungeon.updateDamageQuestProgress(m_dungeon.getQuest().getTargetCount());
                }
                return "Quest finished";
            }
            else
            {
                return "No quest available";
            }
        },
        "Finish the current quest");

    // hesoyam
    m_cheatConsole.registerCommand(
        "hesoyam",
        [this](const std::vector<std::string>& args) -> std::string {
            if (m_character)
            {
                Character* character =
                    GetContext().GetUnitManager()->GetUnitOfType<Character>(m_character->GetId());
                if (character)
                {
                    character->SetHealth(character->GetMaxHealth());
                    character->SetCurrentMana(character->GetMaxMana());
                    character->AddGold(1000);
                    character->AddExp(1000);
                    return "HESOYAM activated";
                }
                else
                {
                    return "Character not found";
                }
            }
            return "Character not initialized";
        },
        "Full health and mana");
}