#pragma once
#include "BaseState.h"
#include "EventManager.h"

enum class GameMode;

enum class MenuType
{
	MAIN,
	PLAY_MODE_SELECT,
	IN_GAME
};

class State_MainMenu : public BaseState
{
public:
	explicit State_MainMenu(StateManager* l_stateManager);
	~State_MainMenu();

	void OnCreate() override;
	void OnDestroy() override;

	void Activate() override;
	void Deactivate() override;

	void Update(const sf::Time& l_time) override;
	void Draw() override;

	void GenerateButtons();

	void LeaveGame(EventDetails* l_details = nullptr);
	void GoBackToMain(EventDetails* l_details = nullptr);
	void GoToInGameMenu(EventDetails* l_details = nullptr);
	void GoToPlayModeSelection(EventDetails* l_details = nullptr);
	void Play(const GameMode p_gameMode) const;
	void Credits() const;
	void Exit() const;

	void MouseMoved(EventDetails* l_details);
	void Validate(EventDetails* l_details = nullptr);
	void MouseValidate(EventDetails* l_details = nullptr);
	void Move(EventDetails* l_details);

	void MuteToggle(EventDetails* l_details = nullptr) const;

private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_gameLogoSprite;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];

	MenuType m_menuType;

	uint8_t m_selected;
};