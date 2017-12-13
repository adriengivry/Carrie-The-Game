#include "State_MainMenu.h"
#include "StateManager.h"
#include "Utilities.h"

State_MainMenu::State_MainMenu(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_buttonPadding(0),
	m_menuType(MenuType::MAIN),
	m_selected(0)
{
}

State_MainMenu::~State_MainMenu() {}

void State_MainMenu::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()
	
	// OnCreate core
	if (textureManager->RequireResource("Menu_Background"))
		m_backgroundSprite.setTexture(*textureManager->GetResource("Menu_Background"));

	if (textureManager->RequireResource("Game_Logo"))
		m_gameLogoSprite.setTexture(*textureManager->GetResource("Game_Logo"));

	Utils::centerOrigin(m_gameLogoSprite);
	m_gameLogoSprite.setPosition(windowCenter.x, 341.9f);

	m_buttonSize = sf::Vector2f(250.0f,75.0f);
	m_buttonPos = sf::Vector2f(windowCenter.x, windowCenter.y - m_buttonSize.y + 280);
	m_buttonPadding = 10; // in px

	GenerateButtons();

	// Adding callbacks
	if (m_stateMgr->GetContext()->m_gameInfo->m_allowMouse)
	{
		evMgr->AddCallback(StateType::MainMenu, "Mouse_Left", &State_MainMenu::MouseValidate, this);
		evMgr->AddCallback(StateType::MainMenu, "Mouse_Move", &State_MainMenu::MouseMoved, this);
	}
	evMgr->AddCallback(StateType::MainMenu, "Mute", &State_MainMenu::MuteToggle, this);
	evMgr->AddCallback(StateType::MainMenu, "Key_Return", &State_MainMenu::Validate, this);
	evMgr->AddCallback(StateType::MainMenu, "Key_Space", &State_MainMenu::Validate, this);
	evMgr->AddCallback(StateType::MainMenu, "Move_Up", &State_MainMenu::Move, this);
	evMgr->AddCallback(StateType::MainMenu, "Move_Down", &State_MainMenu::Move, this);
	evMgr->AddCallback(StateType::MainMenu, "Move_Up_Alternative", &State_MainMenu::Move, this);
	evMgr->AddCallback(StateType::MainMenu, "Move_Down_Alternative", &State_MainMenu::Move, this);
	evMgr->AddCallback(StateType::MainMenu, "Key_Escape", &State_MainMenu::GoBackToMain, this);
}

void State_MainMenu::OnDestroy() {
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	if (m_stateMgr->GetContext()->m_gameInfo->m_allowMouse)
	{
		evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
		evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Move");
	}
	evMgr->RemoveCallback(StateType::MainMenu, "Mute");
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Return");
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Space");
	evMgr->RemoveCallback(StateType::MainMenu, "Move_Up");
	evMgr->RemoveCallback(StateType::MainMenu, "Move_Down");
	evMgr->RemoveCallback(StateType::MainMenu, "Move_Up_Alternative");
	evMgr->RemoveCallback(StateType::MainMenu, "Move_Down_Alternative");
	evMgr->RemoveCallback(StateType::MainMenu, "Key_Escape");
}

void State_MainMenu::Activate(){
	AudioManager* audioManager = m_stateMgr->GetContext()->m_audioManager;
	audioManager->Pause("Game");
	audioManager->PlayMusic("Menu");

	GoBackToMain();

	m_selected = 0;

	if (m_stateMgr->HasState(StateType::Game))
		GoToInGameMenu();
	else
		GoBackToMain();

	const sf::FloatRect rect = m_labels[0].getLocalBounds();
	m_labels[0].setOrigin(rect.left + rect.width / 2.0f,
		rect.top + rect.height / 2.0f);
}

void State_MainMenu::MouseMoved(EventDetails* l_details)
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	for (int i = 0; i < 3; i++)
	{
		if (sf::Mouse::getPosition(*window).x >= m_rects[i].getPosition().x - m_rects[i].getSize().x / 2
			&& sf::Mouse::getPosition(*window).x <= m_rects[i].getPosition().x + m_rects[i].getSize().x / 2
			&& sf::Mouse::getPosition(*window).y >= m_rects[i].getPosition().y - m_rects[i].getSize().y / 2
			&& sf::Mouse::getPosition(*window).y <= m_rects[i].getPosition().y + m_rects[i].getSize().y / 2)
		{
			m_selected = i;
		}
	}
}

void State_MainMenu::Validate(EventDetails* l_details)
{
	m_stateMgr->GetContext()->m_audioManager->PlaySound("Validate");

	switch (m_menuType)
	{
	case MenuType::MAIN:
		if (m_selected == 0)
		{
			if (!m_stateMgr->HasState(StateType::Game))
				GoToPlayModeSelection();
			else
				Play(m_stateMgr->GetContext()->m_gameInfo->m_gameMode);
		}
		else if (m_selected == 1)
			Credits();
		else if (m_selected == 2)
			Exit();
		break;

	case MenuType::PLAY_MODE_SELECT:
		if (m_selected == 0)
			Play(GameMode::ARCADE);
		else if (m_selected == 1)
			Play(GameMode::BOSS_RUSH);
		else if (m_selected == 2)
			GoBackToMain();
		break;

	case MenuType::IN_GAME:
		if (m_selected == 0)
			Play(m_stateMgr->GetContext()->m_gameInfo->m_gameMode);
		else if (m_selected == 1)
			LeaveGame();
		else if (m_selected == 2)
			Exit();
		break;
	}
}

void State_MainMenu::MouseValidate(EventDetails* l_details)
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
	bool hovering = false;

	for (int i = 0; i < 3; i++)
	{
		if (sf::Mouse::getPosition(*window).x >= m_rects[i].getPosition().x - m_rects[i].getSize().x / 2
			&& sf::Mouse::getPosition(*window).x <= m_rects[i].getPosition().x + m_rects[i].getSize().x / 2
			&& sf::Mouse::getPosition(*window).y >= m_rects[i].getPosition().y - m_rects[i].getSize().y / 2
			&& sf::Mouse::getPosition(*window).y <= m_rects[i].getPosition().y + m_rects[i].getSize().y / 2)
		{
			hovering = true;
		}
	}

	if (hovering)
		Validate();
}

void State_MainMenu::Move(EventDetails* l_details)
{
	const sf::Keyboard::Key keyPressed = sf::Keyboard::Key(l_details->m_keyCode);

	switch (keyPressed)
	{
	case sf::Keyboard::Up:
	case sf::Keyboard::W:
		if (m_selected == 0)
			m_selected = 2;
		else
			--m_selected;
		break;

	case sf::Keyboard::Down:
	case sf::Keyboard::S:
		++m_selected;
		if (m_selected > 2)
			m_selected = 0;
		break;

	default:
		break;
	}
}

void State_MainMenu::MuteToggle(EventDetails* l_details) const
{
	m_stateMgr->GetContext()->m_audioManager->ToggleMute();
}

void State_MainMenu::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_backgroundSprite);
	window->draw(m_gameLogoSprite);

	for(int i = 0; i < 3; ++i)
	{
		if (i == m_selected)
		{
			m_labels[i].setFillColor(sf::Color(134, 0, 29));
		}
		else
		{
			m_labels[i].setFillColor(sf::Color::White);
		}

		window->draw(m_labels[i]);
	}
}

void State_MainMenu::GenerateButtons()
{
	std::string str[3];

	switch (m_menuType)
	{
	case MenuType::MAIN:
		str[0] = "PLAY";
		str[1] = "CREDITS";
		str[2] = "EXIT";
		break;

	case MenuType::PLAY_MODE_SELECT:
		str[0] = "ARCADE";
		str[1] = "BOSS RUSH";
		str[2] = "BACK";
		break;

	case MenuType::IN_GAME:
		str[0] = "RESUME";
		str[1] = "GIVE UP";
		str[2] = "EXIT";
		break;
	}

	for (int i = 0; i < 3; ++i)
	{
		const sf::Vector2f buttonPosition(m_buttonPos.x, m_buttonPos.y +
			i * (m_buttonSize.y + m_buttonPadding));
		m_rects[i].setSize(m_buttonSize);
		m_rects[i].setFillColor(sf::Color::Red);

		m_rects[i].setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
		m_rects[i].setPosition(buttonPosition);

		if (m_stateMgr->GetContext()->m_fontManager->RequireResource("Retro_Menu"))
			m_labels[i].setFont(*m_stateMgr->GetContext()->m_fontManager->GetResource("Retro_Menu"));
		m_labels[i].setString(sf::String(str[i]));
		m_labels[i].setCharacterSize(45);


		Utils::centerOrigin(m_labels[i]);

		m_labels[i].setPosition(buttonPosition);
	}
}

void State_MainMenu::LeaveGame(EventDetails* l_details)
{
	m_stateMgr->Remove(StateType::Game);
	GoBackToMain();
}

void State_MainMenu::GoBackToMain(EventDetails* l_details)
{
	m_menuType = MenuType::MAIN;
	GenerateButtons();
}

void State_MainMenu::GoToInGameMenu(EventDetails* l_details)
{
	m_menuType = MenuType::IN_GAME;
	GenerateButtons();
}

void State_MainMenu::GoToPlayModeSelection(EventDetails* l_details)
{
	m_menuType = MenuType::PLAY_MODE_SELECT;
	GenerateButtons();
}

void State_MainMenu::Play(const GameMode p_gameMode) const
{
	m_stateMgr->GetContext()->m_gameInfo->m_gameMode = p_gameMode;
	m_stateMgr->SwitchTo(StateType::Game);
}

void State_MainMenu::Credits() const
{
	m_stateMgr->SwitchTo(StateType::Credits);
}

void State_MainMenu::Exit() const
{
	m_stateMgr->GetContext()->m_wind->Close();
}

void State_MainMenu::Update(const sf::Time& l_time) {}
void State_MainMenu::Deactivate() {}