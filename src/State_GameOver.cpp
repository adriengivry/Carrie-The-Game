#include "State_GameOver.h"
#include "StateManager.h"
#include "Utilities.h"

State_GameOver::State_GameOver(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_timePassed(0.0f) {}

State_GameOver::~State_GameOver() {}

void State_GameOver::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	textureManager->RequireResource("GameOver_Bg");

	m_backgroundSprite.setTexture(*textureManager->GetResource("GameOver_Bg"));

	m_text.setFont(*fontManager->GetResource("Retro"));
	m_text.setString("GAME OVER");
	m_text.setCharacterSize(120);
	m_text.setFillColor(sf::Color::Red);
	
	Utils::centerOrigin(m_text);
	m_text.setPosition(sf::Vector2f(0 - m_text.getLocalBounds().width / 2, windowCenter.y));

	evMgr->AddCallback(StateType::GameOver, "Key_Escape", &State_GameOver::MainMenu, this);
}

void State_GameOver::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::GameOver, "Key_Escape");
}

void State_GameOver::MainMenu(EventDetails* l_details) const
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
	m_stateMgr->Remove(StateType::GameOver);
}

void State_GameOver::Update(const sf::Time& l_time)
{
	m_timePassed += l_time.asSeconds();

	const sf::Vector2u windowCenter = m_stateMgr->GetContext()->m_wind->GetWindowCenter();

	if (m_text.getPosition().x < windowCenter.x)
		m_text.move(1200 * l_time.asSeconds(), 0);
	else
		if (m_timePassed >= 4.0f)
			m_text.move(2000 * l_time.asSeconds(), 0);

	if (m_text.getPosition().x - m_text.getLocalBounds().width / 2 >= windowCenter.x * 2)
		MainMenu();
}

void State_GameOver::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_backgroundSprite);
	window->draw(m_text);
}

void State_GameOver::Activate()
{
	SoundManager* soundManager = m_stateMgr->GetContext()->m_soundManager;
	soundManager->PauseMusics();
	soundManager->PlaySound("GameOver");
	m_stateMgr->Remove(StateType::Game);
}
void State_GameOver::Deactivate()
{
	SoundManager* soundManager = m_stateMgr->GetContext()->m_soundManager;
	soundManager->ClearAll();
}