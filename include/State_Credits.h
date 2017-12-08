#pragma once
#include "BaseState.h"
#include "EventManager.h"

#define CREDIT_NUMBER_OF_LINES 10

class State_Credits : public BaseState
{
	const uint16_t __PIXEL_TO_SCROLL_PER_SECOND_DEFAULT = 750;
	const uint16_t __PIXEL_ACCELERATION_PER_SECOND = 100;
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

	float m_pixelToScrollPerSeconds;

	bool m_fastForward;

	sf::Sprite m_backgroundSprite;
};