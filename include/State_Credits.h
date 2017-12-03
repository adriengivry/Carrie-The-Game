#pragma once
#include "BaseState.h"
#include "EventManager.h"

#define CREDIT_NUMBER_OF_LINES 10

class State_Credits : public BaseState
{
	const int sCREDIT_LINE_PADDING = 40;
	const int sCREDIT_LINE_CHARACTER_SIZE = 35;
public:
	explicit State_Credits(StateManager* l_stateManager);
	~State_Credits();

	void OnCreate() override;
	void OnDestroy() override;

	void MainMenu(EventDetails* l_details = nullptr) const;
	void FastForwardStart(EventDetails* l_details = nullptr);
	void FastForwardStop(EventDetails* l_details = nullptr);

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& l_time) override;
	void Draw() override;
private:
	float m_timePassed;

	bool m_fastForward;

	sf::Sprite m_backgroundSprite;
};