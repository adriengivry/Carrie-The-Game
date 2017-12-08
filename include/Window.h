#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "EventManager.h"

class Window
{
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void BeginDraw();
	void EndDraw();

	void Update();

	bool IsDone() const;
	bool IsFullscreen() const;
	bool IsFocused() const;
	bool IsResizable() const;

	void ToggleFullscreen(EventDetails* l_details);
	void Close(EventDetails* l_details = nullptr);

	void ApplyLetterBoxView(const uint16_t p_iWindowWidth, const uint16_t p_iWindowHeight);

	sf::RenderWindow* GetRenderWindow();
	EventManager* GetEventManager();
	sf::Vector2u GetWindowSize() const;
	sf::Vector2u GetWindowCenter() const;
	sf::FloatRect GetViewSpace() const;
private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Create();

	sf::RenderWindow m_window;
	EventManager m_eventManager;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullscreen;
	bool m_isFocused;
	bool m_isResizable;
	bool m_isVSyncOn;

	uint8_t m_frameRateLimit;
};