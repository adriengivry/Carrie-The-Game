#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "GameInfo.h"

class ActorManager;

struct SharedContext
{
	SharedContext()
		:m_wind(nullptr),
		m_eventManager(nullptr),
		m_textureManager(nullptr),
		m_fontManager(nullptr),
		m_audioManager(nullptr),
		m_soundManager(nullptr),
		m_actorManager(nullptr),
		m_gameInfo(nullptr)
	{}
	Window* m_wind;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
	FontManager* m_fontManager;
	AudioManager* m_audioManager;
	SoundManager* m_soundManager;
	ActorManager* m_actorManager;
	GameInfo* m_gameInfo;
};
