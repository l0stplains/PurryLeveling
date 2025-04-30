#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"
#include "states/ChooseCharacterState.hpp"
#include "states/WorldState.hpp"
#include "units/NavigationGrid.hpp"
#include "units/characters/Assassin.hpp"
#include "units/characters/Berseker.hpp"
#include "units/characters/Fighter.hpp"
#include "units/characters/Mage.hpp"
#include "units/characters/Necromancer.hpp"

// Constructor
ChooseCharacterState::ChooseCharacterState(GameContext& context)
    : State(context),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_squareButtonTexture(GetContext().GetResourceManager()->GetTexture("square_button")),
      m_exitButton(m_squareButtonTexture, {32.f, 32.f}, {0.5f, 0.5f}),
      m_confirmButton(m_buttonTexture,  // at the center of screen
                      {0.f, 0.f}),      // set later
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_boldFont(GetContext().GetResourceManager()->GetFont("main_bold_font")),
      m_menuTitle(m_boldFont),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("choose_character_menu_"
                                                                        "background")),
      m_backgroundSprite(m_backgroundTexture),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("Choose Character State");
}

void ChooseCharacterState::Init()
{
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();

    // title
    m_menuTitle.setString("Choose Character");

    m_menuTitle.setFillColor({255, 255, 255, 144});
    m_menuTitle.setCharacterSize(64);

    m_menuTitle.setOutlineColor(sf::Color::Black);
    m_menuTitle.setOutlineThickness(8.f);

    // Set origin to the center of the text bounds
    sf::FloatRect textBounds = m_menuTitle.getLocalBounds();
    m_menuTitle.setOrigin(textBounds.getCenter());
    m_menuTitle.setPosition({windowSize.x / 2.f, windowSize.y * 0.28f});

    m_exitButton.setText("X", m_font, 24);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    m_confirmButton.setText("Confirm Character", m_font, 30);
    m_confirmButton.setHoverSound(m_buttonHoverSound);
    m_confirmButton.setClickSound(m_buttonClickSound);
    m_confirmButton.setActive(false);

    // Set button callbacks
    m_exitButton.setOnClickCallback([this]() {
        GetContext().GetUnitManager()->Clear();
        m_pendingStateChange = StateChange {StateAction::POP};
    });

    m_confirmButton.setOnClickCallback([this]() { m_showNameModal = true; });

    m_confirmButton.setPosition({windowSize.x / 2.0f, windowSize.y / 2.0f});

    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    NavigationGrid tempNavGrid(windowSize.x, windowSize.y, 51, 51);

    std::vector<std::vector<bool>> grid = {
        {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0},
        {1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0},
        {1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0},
        {1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    if (!tempNavGrid.SetGrid(grid))
    {
        unsigned int passedH = static_cast<unsigned int>(grid.size());
        unsigned int passedW = passedH > 0 ? static_cast<unsigned int>(grid[0].size()) : 0;

        // expected dims from the NavigationGrid
        unsigned int expectedH = tempNavGrid.GetGridHeight();
        unsigned int expectedW = tempNavGrid.GetGridWidth();
        std::cerr << "NavigationGrid::SetGrid failed!\n"
                  << "  expected grid size = " << expectedH << " rows × " << expectedW << " cols\n"
                  << "  passed   grid size = " << passedH << " rows × " << passedW << " cols\n";
    }

    GetContext().debugWorld = true;

    std::cout << tempNavGrid.GetGridWidth() << " " << tempNavGrid.GetGridHeight()
              << " with height in pixel " << tempNavGrid.GetTileHeight() << std::endl;
    GetContext().SetNavigationGrid(tempNavGrid);
    NavigationGrid& navGrid = GetContext().GetNavigationGrid();

    auto fighter =
        std::make_unique<Fighter>("Fighter",
                                  sf::Vector2f(windowSize.x * (1.0f / 6), windowSize.y * 0.76f),
                                  navGrid,
                                  false,
                                  GetContext());
    auto mage = std::make_unique<Mage>("Mage",
                                       sf::Vector2f(windowSize.x * (2.0f / 6), windowSize.y * 0.76f),
                                       navGrid,
                                       false,
                                       GetContext());

    auto assassin =
        std::make_unique<Assassin>("Assassin",
                                   sf::Vector2f(windowSize.x * (3.0f / 6), windowSize.y * 0.76f),
                                   navGrid,
                                   false,
                                   GetContext());
    auto necromancer =
        std::make_unique<Necromancer>("Necromancer",
                                      sf::Vector2f(windowSize.x * (4.0f / 6), windowSize.y * 0.76f),
                                      navGrid,
                                      false,
                                      GetContext());
    auto berseker =
        std::make_unique<Berseker>("Berseker",
                                   sf::Vector2f(windowSize.x * (5.0f / 6), windowSize.y * 0.76f),
                                   navGrid,
                                   false,
                                   GetContext());

    // Load resources (here we go)
    sf::Vector2i fighterFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> fighterTexturePaths = {
        {UnitAnimationType::IDLE, "character_elf_idle"},
        {UnitAnimationType::WALK, "character_elf_walk"},
        {UnitAnimationType::ATTACK, "character_elf_attack"},
        {UnitAnimationType::CHARGED_ATTACK, "character_elf_charged_attack"},
        {UnitAnimationType::JUMP, "character_elf_jump"},
        {UnitAnimationType::DAMAGE, "character_elf_dmg"},
        {UnitAnimationType::DIE, "character_elf_die_spin"}

    };
    std::unordered_map<UnitAnimationType, std::string> fighterShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_elf_idle_shadow"},
        {UnitAnimationType::WALK, "character_elf_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_elf_attack_shadow"},
        {UnitAnimationType::CHARGED_ATTACK, "character_elf_charged_attack_shadow"},
        {UnitAnimationType::JUMP, "character_elf_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_elf_dmg_shadow"},
        {UnitAnimationType::DIE, "character_elf_die_spin_shadow"}

    };
    // Example frame counts (adjust these!)
    std::unordered_map<UnitAnimationType, int> fighterFramesPerAnim = {{UnitAnimationType::IDLE, 16},
                                                                       {UnitAnimationType::WALK, 4},
                                                                       {UnitAnimationType::ATTACK, 4},
                                                                       {UnitAnimationType::JUMP, 4},
                                                                       {UnitAnimationType::DAMAGE, 4},
                                                                       {UnitAnimationType::DIE, 12}};

    // Example durations (in seconds, adjust these!)
    std::unordered_map<UnitAnimationType, float> fighterDurationPerAnim = {
        {UnitAnimationType::IDLE, 6.4f},
        {UnitAnimationType::WALK, 1.6f},
        {UnitAnimationType::ATTACK, 0.8f},
        {UnitAnimationType::JUMP, 0.8f},
        {UnitAnimationType::DAMAGE, 0.8f},
        {UnitAnimationType::DIE, 2.4f}};

    // Example looping status (Idle/Walk usually loop)
    std::unordered_map<UnitAnimationType, bool> fighterLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> fighterDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> fighterDefaultRows = {{UnitAnimationType::DIE, 0}};

    sf::Vector2i mageFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> mageTexturePaths = {
        {UnitAnimationType::IDLE, "character_centaur_idle"},
        {UnitAnimationType::WALK, "character_centaur_jump"},
        {UnitAnimationType::ATTACK, "character_centaur_attack"},
        {UnitAnimationType::JUMP, "character_centaur_jump"},
        {UnitAnimationType::DAMAGE, "character_centaur_dmg"},
        {UnitAnimationType::DIE, "character_centaur_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> mageShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_centaur_idle_shadow"},
        {UnitAnimationType::WALK, "character_centaur_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_centaur_attack_shadow"},
        {UnitAnimationType::JUMP, "character_centaur_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_centaur_dmg_shadow"},
        {UnitAnimationType::DIE, "character_centaur_die_shadow"}

    };
    // Example frame counts (adjust these!)
    std::unordered_map<UnitAnimationType, int> mageFramesPerAnim = {{UnitAnimationType::IDLE, 16},
                                                                    {UnitAnimationType::WALK, 4},
                                                                    {UnitAnimationType::ATTACK, 4},
                                                                    {UnitAnimationType::JUMP, 4},
                                                                    {UnitAnimationType::DAMAGE, 4},
                                                                    {UnitAnimationType::DIE, 12}};

    // Example durations (in seconds, adjust these!)
    std::unordered_map<UnitAnimationType, float> mageDurationPerAnim = {
        {UnitAnimationType::IDLE, 6.4f},
        {UnitAnimationType::WALK, 1.6f},
        {UnitAnimationType::ATTACK, 0.8f},
        {UnitAnimationType::JUMP, 0.8f},
        {UnitAnimationType::DAMAGE, 0.8f},
        {UnitAnimationType::DIE, 2.4f}};

    // Example looping status (Idle/Walk usually loop)
    std::unordered_map<UnitAnimationType, bool> mageLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> mageDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> mageDefaultRows = {{UnitAnimationType::DIE, 0}};

    sf::Vector2i assassinFrameSize(32, 32);  // Example frame size

    std::unordered_map<UnitAnimationType, std::string> assassinTexturePaths = {
        {UnitAnimationType::IDLE, "character_amazon_idle"},
        {UnitAnimationType::WALK, "character_amazon_jump"},
        {UnitAnimationType::ATTACK, "character_amazon_attack"},
        {UnitAnimationType::CHARGED_ATTACK, "character_amazon_charged_attack"},
        {UnitAnimationType::JUMP, "character_amazon_jump"},
        {UnitAnimationType::DAMAGE, "character_amazon_dmg"},
        {UnitAnimationType::DIE, "character_amazon_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> assassinShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_amazon_idle_shadow"},
        {UnitAnimationType::WALK, "character_amazon_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_amazon_attack_shadow"},
        {UnitAnimationType::CHARGED_ATTACK, "character_amazon_charged_attack_shadow"},
        {UnitAnimationType::JUMP, "character_amazon_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_amazon_dmg_shadow"},
        {UnitAnimationType::DIE, "character_amazon_die_shadow"}

    };

    std::unordered_map<UnitAnimationType, int> assassinFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> assassinDurationPerAnim = {
        {UnitAnimationType::IDLE, 6.4f},
        {UnitAnimationType::WALK, 1.6f},
        {UnitAnimationType::ATTACK, 0.8f},
        {UnitAnimationType::JUMP, 0.8f},
        {UnitAnimationType::DAMAGE, 0.8f},
        {UnitAnimationType::DIE, 2.4f}};

    std::unordered_map<UnitAnimationType, bool> assassinLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> assassinDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> assassinDefaultRows = {{UnitAnimationType::DIE, 0}};

    sf::Vector2i                                       necromancerFrameSize(32, 32);
    std::unordered_map<UnitAnimationType, std::string> necromancerTexturePaths = {
        {UnitAnimationType::IDLE, "character_cyclop_idle"},
        {UnitAnimationType::WALK, "character_cyclop_jump"},
        {UnitAnimationType::ATTACK, "character_cyclop_attack"},
        {UnitAnimationType::JUMP, "character_cyclop_jump"},
        {UnitAnimationType::DAMAGE, "character_cyclop_dmg"},
        {UnitAnimationType::DIE, "character_cyclop_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> necromancerShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_cyclop_idle_shadow"},
        {UnitAnimationType::WALK, "character_cyclop_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_cyclop_attack_shadow"},
        {UnitAnimationType::JUMP, "character_cyclop_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_cyclop_dmg_shadow"},
        {UnitAnimationType::DIE, "character_cyclop_die_shadow"}

    };

    std::unordered_map<UnitAnimationType, int> necromancerFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> necromancerDurationPerAnim = {
        {UnitAnimationType::IDLE, 6.4f},
        {UnitAnimationType::WALK, 1.6f},
        {UnitAnimationType::ATTACK, 0.8f},
        {UnitAnimationType::JUMP, 0.8f},
        {UnitAnimationType::DAMAGE, 0.8f},
        {UnitAnimationType::DIE, 2.4f}};

    std::unordered_map<UnitAnimationType, bool> necromancerLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> necromancerDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> necromancerDefaultRows = {{UnitAnimationType::DIE, 0}};

    sf::Vector2i bersekerFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> bersekerTexturePaths = {
        {UnitAnimationType::IDLE, "character_dwarf_idle"},
        {UnitAnimationType::WALK, "character_dwarf_jump"},
        {UnitAnimationType::ATTACK, "character_dwarf_attack"},
        {UnitAnimationType::CHARGED_ATTACK, "character_dwarf_charged_attack"},
        {UnitAnimationType::JUMP, "character_dwarf_jump"},
        {UnitAnimationType::DAMAGE, "character_dwarf_dmg"},
        {UnitAnimationType::DIE, "character_dwarf_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> bersekerShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_dwarf_idle_shadow"},
        {UnitAnimationType::WALK, "character_dwarf_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_dwarf_attack_shadow"},
        {UnitAnimationType::CHARGED_ATTACK, "character_dwarf_charged_attack_shadow"},
        {UnitAnimationType::JUMP, "character_dwarf_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_dwarf_dmg_shadow"},
        {UnitAnimationType::DIE, "character_dwarf_die_shadow"}

    };

    std::unordered_map<UnitAnimationType, int> bersekerFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> bersekerDurationPerAnim = {
        {UnitAnimationType::IDLE, 6.4f},
        {UnitAnimationType::WALK, 1.6f},
        {UnitAnimationType::ATTACK, 0.8f},
        {UnitAnimationType::JUMP, 0.8f},
        {UnitAnimationType::DAMAGE, 0.8f},
        {UnitAnimationType::DIE, 2.4f}};

    std::unordered_map<UnitAnimationType, bool> bersekerLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> bersekerDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> bersekerDefaultRows = {{UnitAnimationType::DIE, 0}};

    fighter->LoadAnimations(fighterTexturePaths,
                            fighterFrameSize,
                            fighterFramesPerAnim,
                            fighterDurationPerAnim,
                            fighterLoopingAnims,
                            fighterDirectionalAnims,
                            fighterDefaultRows,
                            fighterShadowTexturePaths);

    mage->LoadAnimations(mageTexturePaths,
                         mageFrameSize,
                         mageFramesPerAnim,
                         mageDurationPerAnim,
                         mageLoopingAnims,
                         mageDirectionalAnims,
                         mageDefaultRows,
                         mageShadowTexturePaths);

    assassin->LoadAnimations(assassinTexturePaths,
                             assassinFrameSize,
                             assassinFramesPerAnim,
                             assassinDurationPerAnim,
                             assassinLoopingAnims,
                             assassinDirectionalAnims,
                             assassinDefaultRows,
                             assassinShadowTexturePaths);

    necromancer->LoadAnimations(necromancerTexturePaths,
                                necromancerFrameSize,
                                necromancerFramesPerAnim,
                                necromancerDurationPerAnim,
                                necromancerLoopingAnims,
                                necromancerDirectionalAnims,
                                necromancerDefaultRows,
                                necromancerShadowTexturePaths);

    berseker->LoadAnimations(bersekerTexturePaths,
                             bersekerFrameSize,
                             bersekerFramesPerAnim,
                             bersekerDurationPerAnim,
                             bersekerLoopingAnims,
                             bersekerDirectionalAnims,
                             bersekerDefaultRows,
                             bersekerShadowTexturePaths);

    fighter->SetScale(10.f, 10.f);
    mage->SetScale(10.f, 10.f);
    assassin->SetScale(10.f, 10.f);
    necromancer->SetScale(10.f, 10.f);
    berseker->SetScale(10.f, 10.f);

    m_fighterId     = fighter->GetId();
    m_mageId        = mage->GetId();
    m_assassinId    = assassin->GetId();
    m_necromancerId = necromancer->GetId();
    m_bersekerId    = berseker->GetId();

    GetContext().GetUnitManager()->AddUnit(std::move(fighter));
    GetContext().GetUnitManager()->AddUnit(std::move(mage));
    GetContext().GetUnitManager()->AddUnit(std::move(assassin));
    GetContext().GetUnitManager()->AddUnit(std::move(necromancer));
    GetContext().GetUnitManager()->AddUnit(std::move(berseker));
}

State::StateChange ChooseCharacterState::ProcessEvent(const sf::Event& event)
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

State::StateChange ChooseCharacterState::Update(const sf::Time& dt)
{
    sf::RenderWindow* window = GetContext().GetWindow();

    if (!m_showNameModal)
    {
        m_exitButton.update(*window);
        m_confirmButton.update(*window);
    }

    // Check if we have a pending state change from a button callback
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        return std::move(m_pendingStateChange);
    }

    return StateChange {StateAction::NONE};
}

void ChooseCharacterState::Draw(sf::RenderWindow& window)
{
    window.clear({20, 20, 20});

    window.draw(m_backgroundSprite);
    window.draw(m_menuTitle);

    m_exitButton.draw(window);
    m_confirmButton.draw(window);
}

void ChooseCharacterState::RenderUI()
{
    m_confirmButton.setActive(m_selectedCharacterId != -1);
    // 1) Full-screen character selection window (won't steal focus)
    sf::Vector2u     windowSize   = GetContext().GetWindow()->getSize();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;  // ← key flag

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)windowSize.x, (float)windowSize.y));

    if (ImGui::Begin("Character Selection", nullptr, window_flags))
    {
        // — styles for checkboxes —
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.3f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.35f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.9f, 0.9f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));

        const char*  characterNames[] = {"Fighter", "Mage", "Assassin", "Necromancer", "Berserker"};
        unsigned int characterIds[]   = {
            m_fighterId, m_mageId, m_assassinId, m_necromancerId, m_bersekerId};

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

        // — draw the five checkboxes under each sprite —
        for (int i = 0; i < 5; i++)
        {
            float px = windowSize.x * ((i + 1.0f) / 6.0f) - 6.0f;
            float py = windowSize.y * 0.85f;
            float cb = ImGui::GetFrameHeight();

            ImGui::SetCursorPos(ImVec2(px - cb / 2.0f, py));
            std::string lbl   = "##" + std::string(characterNames[i]);
            bool        isSel = (m_selectedCharacterId == characterIds[i]);

            if (ImGui::Checkbox(lbl.c_str(), &isSel))
            {
                if (isSel)
                {
                    m_selectedCharacterId = characterIds[i];
                    if (auto* u = dynamic_cast<AnimatedUnit*>(
                            GetContext().GetUnitManager()->GetUnit(characterIds[i])))
                        u->PlayAnimation(UnitAnimationType::JUMP, nullptr);
                }
                else if (m_selectedCharacterId == (int)characterIds[i])
                {
                    m_selectedCharacterId = -1;
                }
            }

            // label under checkbox
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
            ImGui::SetWindowFontScale(1.2f);
            float tw = ImGui::CalcTextSize(characterNames[i]).x;
            ImGui::SetCursorPos(ImVec2(px - tw / 2.0f, py + cb + 10.0f));
            ImGui::Text("%s", characterNames[i]);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
        }

        // — Info “?” button next to selected character —
        if (m_selectedCharacterId != -1)
        {
            int selIdx = -1;
            for (int i = 0; i < 5; ++i)
                if (characterIds[i] == m_selectedCharacterId)
                    selIdx = i;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.6f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.7f, 0.9f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.5f, 1.0f));

            float px = windowSize.x * ((selIdx + 1.0f) / 6.0f);
            float py = windowSize.y * 0.85f;
            ImGui::SetCursorPos(ImVec2(px + 25.0f, py));

            if (ImGui::Button("?", ImVec2(30, 30)))
            {
                // Urmm acthually you can just flip it twwewewew
                if (m_showCharacterInfo)
                {
                    m_showCharacterInfo       = false;
                    m_justOpenedCharacterInfo = false;
                }
                else
                {
                    m_showCharacterInfo       = true;
                    m_justOpenedCharacterInfo = true;
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Show character information");

            ImGui::PopStyleColor(3);
        }

        ImGui::PopFont();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(5);
    }
    ImGui::End();

    // 2) Modeless Info-Window (only focused the first frame it opens)
    if (m_selectedCharacterId != -1 && m_showCharacterInfo)
    {
        if (m_justOpenedCharacterInfo)
        {
            ImGui::SetNextWindowFocus();
            m_justOpenedCharacterInfo = false;
        }

        ImGuiWindowFlags infoFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_AlwaysAutoResize |
                                     ImGuiWindowFlags_NoSavedSettings;

        const char* characterNamesInfo[] = {"Fighter", "Mage", "Assassin", "Necromancer", "Berserker"};
        unsigned int characterIdsInfo[] = {
            m_fighterId, m_mageId, m_assassinId, m_necromancerId, m_bersekerId};

        int selIdx = -1;
        for (int i = 0; i < 5; ++i)
            if (characterIdsInfo[i] == m_selectedCharacterId)
                selIdx = i;

        float infoX = windowSize.x * ((selIdx + 1.0f) / 6.0f) - 150.0f;
        float infoY = windowSize.y * 0.4f;
        ImGui::SetNextWindowPos(ImVec2(infoX, infoY), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Once);

        std::string title = std::string(characterNamesInfo[selIdx]) + " Info";
        if (ImGui::Begin(title.c_str(), &m_showCharacterInfo, infoFlags))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
            ImGui::TextColored(
                ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Character Class: %s", characterNamesInfo[selIdx]);
            ImGui::Separator();

            switch (selIdx)
            {
                case 0:  // Fighter
                    ImGui::Text("Health: 100");
                    ImGui::Text("Stamina: 80");
                    ImGui::Text("Strength: 9");
                    ImGui::Text("Defense: 7");
                    ImGui::Text("Special: Strong against physical attacks");
                    ImGui::Text("Weakness: Low magic resistance");
                    ImGui::Separator();
                    ImGui::TextWrapped("A powerful melee fighter with high physical damage and "
                                       "durability.");
                    break;
                case 1:  // Mage
                    ImGui::Text("Health: 70");
                    ImGui::Text("Mana: 120");
                    ImGui::Text("Intelligence: 10");
                    ImGui::Text("Magic Defense: 8");
                    ImGui::Text("Special: Area damage spells");
                    ImGui::Text("Weakness: Low physical defense");
                    ImGui::Separator();
                    ImGui::TextWrapped("A master of arcane arts with powerful area-of-effect "
                                       "spells.");
                    break;
                case 2:  // Assassin
                    ImGui::Text("Health: 80");
                    ImGui::Text("Energy: 100");
                    ImGui::Text("Agility: 10");
                    ImGui::Text("Critical Rate: 25%%");
                    ImGui::Text("Special: High dodge chance");
                    ImGui::Text("Weakness: Low health pool");
                    ImGui::Separator();
                    ImGui::TextWrapped("A swift dealer of deadly strikes with high critical damage "
                                       "potential.");
                    break;
                case 3:  // Necromancer
                    ImGui::Text("Health: 75");
                    ImGui::Text("Mana: 110");
                    ImGui::Text("Dark Power: 9");
                    ImGui::Text("Summon Capacity: 3");
                    ImGui::Text("Special: Can summon undead minions");
                    ImGui::Text("Weakness: Vulnerable while summoning");
                    ImGui::Separator();
                    ImGui::TextWrapped("Commands the powers of death to summon minions and drain "
                                       "life force.");
                    break;
                case 4:  // Berserker
                    ImGui::Text("Health: 120");
                    ImGui::Text("Rage: 100");
                    ImGui::Text("Strength: 10");
                    ImGui::Text("Toughness: 8");
                    ImGui::Text("Special: Damage increases as health lowers");
                    ImGui::Text("Weakness: Hard to control when enraged");
                    ImGui::Separator();
                    ImGui::TextWrapped("A fearsome warrior whose power grows with rage and battle "
                                       "damage.");
                    break;
            }

            ImGui::Separator();
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 120) * 0.5f);
            if (ImGui::Button("Close", ImVec2(120, 30)))
                m_showCharacterInfo = false;

            ImGui::PopStyleColor();
        }
        ImGui::End();
    }
    if (m_showNameModal)
    {
        ImGui::OpenPopup("Enter Username");
    }

    // 2) Before BeginPopupModal, position it at the center on first appearance:
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                            ImGuiCond_Appearing,
                            ImVec2(0.5f, 0.5f)  // pivot in the middle of the popup
    );

    // 3) The modal itself
    if (ImGui::BeginPopupModal("Enter Username", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Please type your name:");
        ImGui::Spacing();

        // input field
        ImGui::InputText("##username", m_userNameBuffer, sizeof(m_userNameBuffer));

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Buttons
        if (ImGui::Button("Finish", ImVec2(100, 0)))
        {
            GetContext().SetCharacterId(m_selectedCharacterId);
            m_showNameModal = false;

            // Clear up the units except the selected one

            unsigned int characterIds[] = {
                m_fighterId, m_mageId, m_assassinId, m_necromancerId, m_bersekerId};

            for (const auto& id : characterIds)
            {
                if (m_selectedCharacterId != id)
                {
                    GetContext().GetUnitManager()->RemoveUnit(id);
                }
            }

            Character* character =
                GetContext().GetUnitManager()->GetUnitOfType<Character>(m_selectedCharacterId);
            character->SetName(m_userNameBuffer);

            m_pendingStateChange =
                StateChange {StateAction::REPLACE, std::make_unique<WorldState>(GetContext())};
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 0)))
        {
            m_showNameModal = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void ChooseCharacterState::Exit() {}