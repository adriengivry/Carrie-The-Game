#include "Game.h"
Game::Game(): m_window("SFML-GameFramework", sf::Vector2u(1920, 1080)), m_stateManager(&m_context){
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

	m_fontManager.RequireResource("Retro");
	m_fontManager.RequireResource("Retro_Menu");
	m_fontManager.RequireResource("Console");

	m_textureManager.RequireResource("Cursor");
	m_cursor.setTexture(*m_textureManager.GetResource("Cursor"));
	m_cursor.setScale(0.7f, 0.7f);
	Utils::ShowCursor(false);
	m_soundManager.SetAudioManager(&m_audioManager);
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