#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class StateManager;

#define LOAD_UTILITY()\
	const sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetWindowSize();\
	const sf::Vector2u windowCenter = m_stateMgr->GetContext()->m_wind->GetWindowCenter();\
	TextureManager* textureManager = m_stateMgr->GetContext()->m_textureManager;\
	FontManager* fontManager = m_stateMgr->GetContext()->m_fontManager;\
	AudioManager* audioManager = m_stateMgr->GetContext()->m_audioManager;\
	SoundManager* soundManager = m_stateMgr->GetContext()->m_soundManager;\
	ActorManager* actorManager = m_stateMgr->GetContext()->m_actorManager;\
	GameInfo* gameInfo = m_stateMgr->GetContext()->m_gameInfo;\
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;

class BaseState
{
	friend class StateManager;
public:
	explicit BaseState(StateManager* l_stateManager) :
		m_stateMgr(l_stateManager),
		m_transparent(false),
		m_transcendent(false){}

	virtual ~BaseState() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& l_time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& l_transparent) { m_transparent = l_transparent; }
	bool IsTransparent() const { return m_transparent; }
	void SetTranscendent(const bool& l_transcendence) { m_transcendent = l_transcendence; }
	bool IsTranscendent() const { return m_transcendent; }
	StateManager* GetStateManager() const { return m_stateMgr; }
	sf::View& GetView() { return m_view; }

protected:
	StateManager* m_stateMgr;
	bool m_transparent;
	bool m_transcendent;
	sf::View m_view;
};