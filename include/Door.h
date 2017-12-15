#pragma once
#include "Actor.h"

class Door : public Actor
{
private:
	const float			__DOOR_ACTIVATION_ZONE = 80;

public:
	explicit Door(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0, const bool p_answer = false, const bool p_shopDoor = false);
	~Door();

	void Activate();
	void Desactivate();

	void Use() const;

	void Draw() const override;
	void DrawLabel() const;

	bool IsActivated() const;

	void SelectCurse() const;

	void Update(const sf::Time& l_time) override;
private:
	bool m_shopDoor;
	bool m_activated;
	bool m_alreadyGetUsed;
	bool m_answer;

	sf::CircleShape m_activationZoneDebug;
};
