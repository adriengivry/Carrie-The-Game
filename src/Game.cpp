#include "Game.h"
Game::Game(): m_window("Carrie - The Game", sf::Vector2u(1920, 1080)), m_stateManager(&m_context){
	m_clock.restart();
	srand(time(nullptr));

	m_context.m_wind = &m_window;
	m_context.m_eventManager = m_window.GetEventManager();
	m_context.m_textureManager = &m_textureManager;
	m_context.m_fontManager = &m_fontManager;
	m_context.m_audioManager = &m_audioManager;
	m_context.m_soundManager = &m_soundManager;
	m_context.m_actorManager = &m_actorManager;
	m_context.m_gameInfo = &m_gameInfo;

	if (m_textureManager.RequireResource("Cursor"))
		m_cursor.setTexture(*m_textureManager.GetResource("Cursor"));

	m_cursor.setScale(0.7f, 0.7f);
	Utils::ShowCursor(false);
	m_soundManager.SetAudioManager(&m_audioManager);

	if (m_gameInfo.m_debugMode)
		m_stateManager.SwitchTo(StateType::Game);
	else
		m_stateManager.SwitchTo(StateType::Intro);
}

Game::~Game()
{
	std::cout << "Game played : " << m_gameInfo.m_gameStarted << std::endl;
}

sf::Time Game::GetElapsed() const { return m_clock.getElapsedTime(); }
void Game::RestartClock() { m_elapsed = m_clock.restart(); }
Window* Game::GetWindow() { return &m_window; }

void Game::Update()
{
	// Avoid for example sprite to move a big distance due to delta time and then ignore collisions
	if (m_elapsed > sf::seconds(1 / 15.0f))
		m_elapsed = sf::seconds(1 / 15.0f);

	m_window.Update();
	m_cursor.setPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
	m_stateManager.Update(m_elapsed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && m_gameInfo.m_debugMode)
		m_soundManager.PauseMusics();
}

void Game::Render()
{
	m_window.BeginDraw();
	m_stateManager.Draw();
	if (m_gameInfo.m_allowMouse)
		m_window.GetRenderWindow()->draw(m_cursor);
	m_window.EndDraw();
}

void Game::LateUpdate()
{
	m_stateManager.ProcessRequests();
	RestartClock();
}

void Game::ApplyLetterBoxView(const uint16_t p_iWindowWidth, const uint16_t p_iWindowHeight)
{
	const float windowRatio = p_iWindowWidth / static_cast<float>(p_iWindowHeight);
	const float viewRatio = m_window.GetRenderWindow()->getView().getSize().x / static_cast<float>(this->m_window.GetRenderWindow()->getView().getSize().y);
	float sizeX = 1.0f;
	float sizeY = 1.0f;
	float posX = 0.0f;
	float posY = 0.0f;
	bool horizontalSpacing = true;

	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing)
	{
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.0f;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.0f;
	}

	sf::View view;
	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
	m_window.GetRenderWindow()->setView(this->m_window.GetRenderWindow()->getView());
}
