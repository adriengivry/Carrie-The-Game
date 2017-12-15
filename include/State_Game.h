#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "ActorManager.h"

struct CurseDetails
{
	bool m_reverseMovement;
	uint8_t m_slowerCarrie = 0;
	uint8_t m_slowerProjectiles = 0;
	uint8_t m_weakerProjectiles = 0;
	uint8_t m_reducedPrecision = 0;

	void Reset()
	{
		m_reverseMovement = false;
		m_slowerCarrie = 0;
		m_slowerProjectiles = 0;
		m_weakerProjectiles = 0;
		m_reducedPrecision = 0;
	}
};

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
	bool CanInteract() const;

	void Update(const sf::Time& l_time) override;
	void Draw() override;
	void DrawUserInterface();
	void DrawConsole() const;
	void DrawFPS() const;

	void UpdateCurseDetails();

	void MainMenu(EventDetails* l_details) const;
	void Pause(EventDetails* l_details) const;
	void KillEnemies(EventDetails* l_details) const;
	void AddMoney(EventDetails* l_details) const;
	void AddLife(EventDetails* l_details) const;
	void AddCarrot(EventDetails* l_details) const;
	void ToggleDebugMode(EventDetails* l_details) const;
private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_backgroundEdgesSprites;

	bool m_isShopingLevel;

	uint16_t m_currentLevel;

	sf::Sprite m_curseIcon[5];

	float m_whiteRectOpacity;
	sf::RectangleShape m_whiteRect;

	uint16_t m_framerate;

	CurseDetails m_curseDetails;

	sf::Text m_levelEndText;
	float m_sliderPauseTimer;

	sf::Sprite m_transitionSlider;

	bool m_startTransition;
	bool m_transitionEnd;

	sf::Vector2f m_increment;
};