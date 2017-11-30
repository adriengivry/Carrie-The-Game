#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_GameOver : public BaseState
{
public:
	explicit State_GameOver(StateManager* l_stateManager);
	~State_GameOver();

	void OnCreate() override;
	void OnDestroy() override;

	void MainMenu(EventDetails* l_details = nullptr) const;

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& l_time) override;
	void Draw() override;
private:
	float m_timePassed;

	sf::Sprite m_backgroundSprite;

	sf::Text m_text;
};