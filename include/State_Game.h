#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "ActorManager.h"

class State_Game : public BaseState
{
public:
	explicit State_Game(StateManager* l_stateManager);
	~State_Game();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void GameOver() const;

	bool LevelCompleted() const;

	void Update(const sf::Time& l_time) override;
	void Draw() override;

	void MainMenu(EventDetails* l_details) const;
	void Pause(EventDetails* l_details) const;
private:
	sf::Sprite m_backgroundSprite;

	sf::Vector2f m_increment;
};