#include "State_Credits.h"
#include "StateManager.h"
#include "Utilities.h"

State_Credits::State_Credits(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_timePassed(0.0f), 
	m_pixelToScrollPerSeconds(0),
	m_fastForward(false)
{
}

State_Credits::~State_Credits() {}

void State_Credits::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	if (textureManager->RequireResource("Credits_Slide"))
		m_backgroundSprite.setTexture(*textureManager->GetResource("Credits_Slide"));

	// Adding callbacks
	evMgr->AddCallback(StateType::Credits, "Key_Escape", &State_Credits::MainMenu, this);
	evMgr->AddCallback(StateType::Credits, "Fast_Forward_Start", &State_Credits::FastForwardStart, this);
	evMgr->AddCallback(StateType::Credits, "Fast_Forward_Stop", &State_Credits::FastForwardStop, this);
}

void State_Credits::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Credits, "Key_Escape");
	evMgr->RemoveCallback(StateType::Credits, "Fast_Forward_Start");
	evMgr->RemoveCallback(StateType::Credits, "Fast_Forward_Stop");
}

void State_Credits::MainMenu(EventDetails* l_details) const
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Credits::FastForwardStart(EventDetails* l_details)
{
	m_fastForward = true;
}

void State_Credits::FastForwardStop(EventDetails* l_details)
{
	m_fastForward = false;
}

void State_Credits::Update(const sf::Time& l_time)
{
	if (m_backgroundSprite.getPosition().y <= -3844)
	{
		m_timePassed += l_time.asSeconds();
		if (m_timePassed >= 1.0f)
			MainMenu();
	}
	else
	{
		if (m_pixelToScrollPerSeconds >= __PIXEL_ACCELERATION_PER_SECOND)
			m_pixelToScrollPerSeconds = __PIXEL_ACCELERATION_PER_SECOND;
		else
			m_pixelToScrollPerSeconds += __PIXEL_ACCELERATION_PER_SECOND * l_time.asSeconds();

		float pixelToScrollPerSecond = m_pixelToScrollPerSeconds;

		if (m_fastForward)
			pixelToScrollPerSecond *= 2.5f;

		m_backgroundSprite.move(0, -pixelToScrollPerSecond * l_time.asSeconds());
	}
}

void State_Credits::Draw() 
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_backgroundSprite);
}

void State_Credits::Activate()
{
	const sf::Vector2u windowCenter = m_stateMgr->GetContext()->m_wind->GetWindowCenter();

	m_backgroundSprite.setPosition(0.f, 0.f);
}

void State_Credits::Deactivate() {}