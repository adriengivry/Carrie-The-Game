#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "TextureManager.h"

class State_Intro : public BaseState
{
public:
	explicit State_Intro(StateManager* l_stateManager);
	~State_Intro();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& l_time) override;
	void Draw() override;

	void Continue(EventDetails* l_details);
private:
	sf::Text m_text;

	sf::Sprite m_backgroundSprite;
	sf::Sprite m_introSprite;
	
	float m_logoScaleIncrement;

	float m_timePassed;

	float m_startTextOpacityIncrement;
	float m_startTextOpacity;

	sf::View m_newView;
	float m_viewZoom;

	bool m_continue = false;
};