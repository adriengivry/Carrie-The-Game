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
	void DrawUserInterface();
	void DrawConsole() const;

	void MainMenu(EventDetails* l_details) const;
	void Pause(EventDetails* l_details) const;
private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_backgroundEdgesSprites;

	sf::Sprite m_curseIcon[4];

	float m_whiteRectOpacity;
	sf::RectangleShape m_whiteRect;

	sf::Text m_curseText;
	float m_sliderPauseTimer;

	sf::Sprite m_transitionSlider;

	bool m_startTransition;
	bool m_transitionEnd;

	sf::Vector2f m_increment;
};