#include "State_Paused.h"
#include "StateManager.h"
#include "Utilities.h"

State_Paused::State_Paused(StateManager* l_stateManager)
	: BaseState(l_stateManager) {}

State_Paused::~State_Paused() {}

void State_Paused::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	SetTransparent(true);

	if (fontManager->RequireResource("Retro_Menu"))
		m_text.setFont(*fontManager->GetResource("Retro_Menu"));
	m_text.setString(sf::String("PAUSED"));
	m_text.setCharacterSize(70);
	m_text.setStyle(sf::Text::Bold);

	Utils::centerOrigin(m_text);
	m_text.setPosition(windowCenter.x, windowCenter.y);

	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0,0);
	m_rect.setFillColor(sf::Color(0,0,0,150));

	// Adding callbacks
	evMgr->AddCallback(StateType::Paused,"Key_P",&State_Paused::Unpause,this);
}

void State_Paused::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Paused,"Key_P");
}

void State_Paused::Draw()
{
	sf::RenderWindow* wind = m_stateMgr->GetContext()->m_wind->GetRenderWindow();
	wind->draw(m_rect);
	wind->draw(m_text);
}

void State_Paused::Unpause(EventDetails* l_details) const
{
	m_stateMgr->SwitchTo(StateType::Game);
}

void State_Paused::Activate()
{
	AudioManager* audioManager = m_stateMgr->GetContext()->m_audioManager;
	audioManager->Pause("Game");
}

void State_Paused::Deactivate() {}
void State_Paused::Update(const sf::Time& l_time) {}