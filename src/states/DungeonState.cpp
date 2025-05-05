#include "states/DungeonState.hpp"

// Constructor
DungeonState::DungeonState(GameContext& context, DimensionType dimension, Dungeon& dungeon)
    : State(context),
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
      m_questInfo(context, 250.0f, 150.0f),  // Initialize QuestInfo with reasonable size
      m_chamberExitArea(GetContext().GetResourceManager()->GetTexture("empty_prop")),
      m_dungeon(dungeon),
      m_chamber(&dungeon.getChamber(0)),
      m_chamberExitPointer(GetContext().GetResourceManager()->GetTexture("pointer_prop"),
                           {1160.f, 540.f},
                           {4.f, 4.f}),
      m_bossHealthBar(120.f),
      m_pendingStateChange({StateAction::NONE})
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

    // setup door enter area
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

    m_attackButton.setOnClickCallback([this]() {
        Mob*         closestMob = getClosestUnitOfType<Mob>(m_mobsID, m_character->GetPosition());
        unsigned int targetId   = closestMob->GetId();
        if (closestMob == nullptr)
        {
            std::cerr << "No mobs available to attack!" << std::endl;
            return;
        }
        // set callback to trigger
        m_turnQueue.pop();
        m_isPlayerTurn                 = false;
        std::function<void()> callback = [this, targetId] {
            m_triggerActionTurn = true;
            m_turnQueue.push(m_character->GetId());
        };

        std::function<void()> onKill = [this, targetId] {
            Mob* attackedMob = GetContext().GetUnitManager()->GetUnitOfType<Mob>(targetId);
            if (attackedMob)
            {
                m_mobsID.erase(std::remove(m_mobsID.begin(), m_mobsID.end(), targetId),
                               m_mobsID.end());
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

    // setup turn queue
    m_turnQueue.push(m_character->GetId());
    for (auto id : m_mobsID)
    {
        m_turnQueue.push(id);
    }

    m_triggerActionTurn = false;
}

State::StateChange DungeonState::ProcessEvent(const sf::Event& event)
{
    // Reset pending state change if it was handled
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        StateChange change   = std::move(m_pendingStateChange);
        m_pendingStateChange = {StateAction::NONE};
        return change;
    }

    return StateChange {};
}

State::StateChange DungeonState::Update(const sf::Time& dt)
{
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
        return StateChange {StateAction::POP};
    }

    // checks if mob clears
    if (!m_walkToExit && m_mobsID.size() == 0 && m_turnQueue.front() == m_character->GetId() &&
        !m_isTransitioning)
    {
        m_walkToExit = true;
        m_character->SetControlledByPlayer(true);
        std::cout << "MOB CLEARED" << std::endl;

        // Update the quest progress when mobs are cleared
        if (m_dungeon.getQuest().getType() == QuestType::KILL)
        {
            // TODO: update
            m_dungeon.updateKillQuestProgress(0);
        }

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
            if (m_dungeon.isQuestCompleted())
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
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Exception: " << e.what() << std::endl;
                        std::cerr << "Backpack is full, couldn't add quest reward item!" << std::endl;
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
    // m_battleUnitInfo.render(*m_character);
    if (m_chamber->getIsBossRoom() && m_mobsID.size() != 0)
    {
        Unit* boss = GetContext().GetUnitManager()->GetUnitOfType<Unit>(m_mobsID[0]);
        m_bossHealthBar.render(*boss);
    }
    // Render battle unit info
    // m_battleUnitInfo.render(*m_character);

    // Render quest info
    m_questInfo.render(m_dungeon);

    // Display exit confirmation popup
    if (m_showExitPopup)
    {
        ImGui::OpenPopup("Surrender Confirmation");
        m_showExitPopup = false;  // prevent reopening every frame
    }
    if (ImGui::BeginPopupModal("Surrender Confirmation",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Do you really want to surrender?");
        ImGui::Dummy(ImVec2(0, 4.0f));  // Add some space
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 4.0f));  // Add some space
        float buttonWidth = 60.0f;
        float buttonPosX  = (ImGui::GetContentRegionAvail().x - 2 * buttonWidth - 10.0f);
        ImGui::SetCursorPosX(buttonPosX);
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red color
        // ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));  //
        // Lighter red ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        // // Darker red

        if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
        {
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
        // ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::SetCursorPosX(buttonPosX + buttonWidth + 10.0f);

        if (ImGui::Button("No", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void DungeonState::Pause() {}

void DungeonState::Resume() {}

void DungeonState::Exit()
{
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