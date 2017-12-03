#pragma once
#include "Actor.h"

class Door : public Actor
{
private:
	const std::string	__DOOR_TEXTURE = "Door";
	const float			__DOOR_ACTIVATION_ZONE = 80;

public:
	explicit Door(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0, const bool p_answer = false);
	~Door();

	void Activate();
	void Desactivate();

	void Use();

	void Draw() const override;
	void DrawLabel() const;

	bool IsActivated() const;

	void SelectCurse() const;

	void Update(const sf::Time& l_time) override;
private:
	bool m_activated;

	bool m_answer;
};
