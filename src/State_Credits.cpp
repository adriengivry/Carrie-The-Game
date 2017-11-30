#include "State_Credits.h"
#include "StateManager.h"
#include "Utilities.h"

State_Credits::State_Credits(StateManager* l_stateManager) :
	BaseState(l_stateManager),
	m_timePassed(0.0f),
	m_fastForward(false)
{
}

State_Credits::~State_Credits() {}

void State_Credits::OnCreate()
{
	// Create quick access variable to shared managers
	LOAD_UTILITY()

	// OnCreate core
	textureManager->RequireResource("Credits_Bg");
	textureManager->RequireResource("Game_Logo");

	m_backgroundSprite.setTexture(*textureManager->GetResource("Credits_Bg"));
	m_gameLogoSprite.setTexture(*textureManager->GetResource("Game_Logo"));

	for(int i = 0; i < CREDIT_NUMBER_OF_LINES; ++i)
	{
		m_creditLine[i].setFont(*fontManager->GetResource("Retro"));
		m_creditLine[i].setCharacterSize(sCREDIT_LINE_CHARACTER_SIZE);
		m_creditLine[i].setFillColor(sf::Color::White);
	}

	m_creditLine[0].setString("-- Game Programmer --");
	m_creditLine[1].setString("Adrien GIVRY");
	m_creditLine[2].setString("");
	m_creditLine[3].setString("-- Game Designer --");
	m_creditLine[4].setString("Adrien GIVRY");
	m_creditLine[5].setString("");
	m_creditLine[6].setString("-- Game Artist --");
	m_creditLine[7].setString("Adrien GIVRY");
	m_creditLine[8].setString("");
	m_creditLine[9].setString("Made in C++ with SFML");

	m_creditLine[0].setFillColor(sf::Color(25, 150, 255));
	m_creditLine[3].setFillColor(sf::Color(25, 150, 255));
	m_creditLine[6].setFillColor(sf::Color(25, 150, 255));
	m_creditLine[9].setFillColor(sf::Color::Yellow);

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
	const sf::Vector2u windowCenter = m_stateMgr->GetContext()->m_wind->GetWindowCenter();

	const int16_t pixelToScrollPerSecond = m_fastForward ? 300 : 100;

	for(int i = 0; i < CREDIT_NUMBER_OF_LINES; ++i)
		m_creditLine[i].move(0, -pixelToScrollPerSecond * l_time.asSeconds());

	if (m_creditLine[CREDIT_NUMBER_OF_LINES - 1].getPosition().y + m_creditLine[CREDIT_NUMBER_OF_LINES - 1].getLocalBounds().height <= 0)
	{
		if (m_gameLogoSprite.getPosition().y >= windowCenter.y)
			m_gameLogoSprite.move(0, -pixelToScrollPerSecond * l_time.asSeconds());
		else
		{
			m_timePassed += l_time.asSeconds();
			if (m_timePassed >= 2.0f)
				MainMenu();
		}
	}
}

void State_Credits::Draw() 
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->m_wind->GetRenderWindow();

	window->draw(m_backgroundSprite);

	for(int i = 0; i < CREDIT_NUMBER_OF_LINES; ++i)
		window->draw(m_creditLine[i]);

	window->draw(m_gameLogoSprite);
}

void State_Credits::Activate()
{
	const sf::Vector2u windowCenter = m_stateMgr->GetContext()->m_wind->GetWindowCenter();

	for (int i = 0; i < CREDIT_NUMBER_OF_LINES; ++i)
	{
		const sf::FloatRect textRect = m_creditLine[i].getLocalBounds();
		m_creditLine[i].setOrigin(textRect.left + textRect.width / 2.0f, 0);
		m_creditLine[i].setPosition(windowCenter.x, windowCenter.y * 2 + i * sCREDIT_LINE_PADDING);
	}

	TextureManager* textureManager = m_stateMgr->GetContext()->m_textureManager;
	Utils::centerOrigin(m_gameLogoSprite);
	m_gameLogoSprite.setPosition(windowCenter.x, windowCenter.y * 2.0f + textureManager->GetResource("Game_Logo")->getSize().y / 2.0f);
}

void State_Credits::Deactivate() {}