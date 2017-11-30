#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Paused : public BaseState
{
public:
	explicit State_Paused(StateManager* l_stateManager);
	~State_Paused();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& l_time) override;
	void Draw() override;

	void Unpause(EventDetails* l_details) const;
private:
	sf::Text m_text;
	sf::RectangleShape m_rect;
};