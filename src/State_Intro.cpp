#include "State_Intro.h"
#include "StateManager.h"
#include "Utilities.h"

State_Intro::State_Intro(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_logoScaleIncrement(0.1),
	m_timePassed(0.0f),
	m_startTextOpacityIncrement(300),
	m_startTextOpacity(0),
	m_viewZoom(0.1)
{
}

State_Intro::~State_Intro() {}

void State_Intro::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	if (textureManager->RequireResource("Menu_Background"))
		m_backgroundSprite.setTexture(*textureManager->GetResource("Menu_Background"));

	if (textureManager->RequireResource("Game_Logo"))
		m_introSprite.setTexture(*textureManager->GetResource("Game_Logo"));

	Utils::centerOrigin(m_introSprite);
	m_introSprite.setPosition(windowSize.x / 2.0f, windowSize.y);

	if (fontManager->RequireResource("Retro_Menu"))
		m_text.setFont(*fontManager->GetResource("Retro_Menu"));

	m_text.setString("PRESS SPACE TO CONTINUE");
	m_text.setCharacterSize(25);
	m_text.setFillColor(sf::Color(255, 255, 255, m_startTextOpacity));
	Utils::centerOrigin(m_text);
	m_text.setPosition(windowCenter.x, windowCenter.y + 200);

	// Adding callbacks
	evMgr->AddCallback(StateType::Intro, "Key_Space", &State_Intro::Continue, this);
}

void State_Intro::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Intro,"Key_Space");
}

void State_Intro::Update(const sf::Time& l_time)
{
	m_timePassed += l_time.asSeconds();

	if (!m_continue)
	{

		if (m_timePassed < 2.0f)
		{
			// Less than five seconds.
			m_introSprite.setScale(0.5f * m_timePassed, 0.5f * m_timePassed);
			m_introSprite.setColor(sf::Color(m_introSprite.getColor().r, m_introSprite.getColor().g, m_introSprite.getColor().b, 127.5 * m_timePassed));

			m_introSprite.setPosition(
				m_introSprite.getPosition().x,
				m_introSprite.getPosition().y - 320 * l_time.asSeconds());
		}

		if (m_timePassed >= 2.5f)
		{
			sf::Color currentColor = m_text.getFillColor();

			if (m_introSprite.getScale().x >= 1.1f)
			{
				m_introSprite.setScale(1.1f, 1.1f);
				m_logoScaleIncrement *= -1;
			}
			else if (m_introSprite.getScale().x <= 0.9f)
			{
				m_introSprite.setScale(0.9f, 0.9f);
				m_logoScaleIncrement *= -1;
			}

			m_introSprite.setScale(sf::Vector2f(m_introSprite.getScale().x + m_logoScaleIncrement * l_time.asSeconds(), m_introSprite.getScale().y + m_logoScaleIncrement * l_time.asSeconds()));

			if (currentColor.a >= 250 && m_startTextOpacityIncrement > 0 || currentColor.a <= 50 && m_startTextOpacityIncrement < 0)
				m_startTextOpacityIncrement *= -1;

			m_startTextOpacity += m_startTextOpacityIncrement * l_time.asSeconds();

			currentColor.a = m_startTextOpacity;

			m_text.setFillColor(currentColor);
		}
	} else if (m_continue)
	{
		if (m_introSprite.getScale().x > 1.0f)
			m_logoScaleIncrement = -abs(m_logoScaleIncrement);
		else if (m_introSprite.getScale().x < 1.0f)
			m_logoScaleIncrement = abs(m_logoScaleIncrement);

		m_introSprite.setScale(sf::Vector2f(m_introSprite.getScale().x + m_logoScaleIncrement * l_time.asSeconds(), m_introSprite.getScale().y + m_logoScaleIncrement * l_time.asSeconds()));

		sf::Color color = m_text.getFillColor();
		color.a = 255 - 255 * m_timePassed;
		m_text.setFillColor(color);

		m_introSprite.setPosition(
			m_introSprite.getPosition().x,
			m_introSprite.getPosition().y - 100 * l_time.asSeconds());

		if (m_timePassed >= 1.0f)
		{
			m_stateMgr->SwitchTo(StateType::MainMenu);
			m_stateMgr->Remove(StateType::Intro);
		}
	}
}

void State_Intro::Draw(){
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_backgroundSprite);

	window->draw(m_introSprite);
	if(m_timePassed >= 2.5f || m_continue)
		window->draw(m_text);
}

void State_Intro::Continue(EventDetails* l_details)
{
	if (m_timePassed >= 2.5f)
	{
		m_stateMgr->GetContext()->m_audioManager->PlaySound("Validate");
		m_continue = true;
		m_timePassed = 0;
	}
}

void State_Intro::Activate()
{
	AudioManager* audioManager = m_stateMgr->GetContext()->m_audioManager;
	audioManager->PlayMusic("Menu");
}
void State_Intro::Deactivate() {}