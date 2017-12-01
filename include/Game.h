#pragma once
#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "AudioManager.h"
#include "GameInfo.h"
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed() const;

	Window* GetWindow();
private:
	SharedContext m_context;
	Window m_window;
	StateManager m_stateManager;
	TextureManager m_textureManager;
	FontManager m_fontManager;
	AudioManager m_audioManager;
	SoundManager m_soundManager;
	ActorManager m_actorManager;
	GameInfo m_gameInfo;
	sf::Clock m_clock;
	sf::Time m_elapsed;
	sf::Sprite m_cursor;

	void RestartClock();
};
