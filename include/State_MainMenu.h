#pragma once
#include "BaseState.h"
#include "EventManager.h"

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

	void Play() const;
	void Credits() const;
	void Exit() const;

	void MouseMoved(EventDetails* l_details);
	void Validate(EventDetails* l_details = nullptr) const;
	void Move(EventDetails* l_details);

private:
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_gameLogoSprite;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];

	uint8_t m_selected;
};