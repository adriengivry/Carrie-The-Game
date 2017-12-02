#pragma once
#include "Actor.h"

class Door : public Actor
{
private:
	const std::string	__DOOR_TEXTURE = "Door";
	const float			__DOOR_ACTIVATION_ZONE = 60;

public:
	explicit Door(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Door();

	void Activate();
	void Desactivate();

	void Use();

	bool IsActivated() const;

	void Update(const sf::Time& l_time) override;
private:
	bool m_activated;
};
