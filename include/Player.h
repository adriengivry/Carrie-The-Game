#pragma once

#include "Actor.h"
#include "Projectile.h"

class Player : public Actor
{
	const std::string	__PLAYER_TEXTURE = "Front";
	const float			__PLAYER_SPEED	 = 400;

public:
	explicit Player(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Player();

	void React(EventDetails* l_details);
	void Unreact(EventDetails* l_details);
	void Move(const sf::Time& l_time) override;
	void Update(const sf::Time& l_time) override;
	void StopControl();
	void Draw() const override;

	void Fire(EventDetails* l_details);

private:
	bool m_moveLeft;
	bool m_moveRight;
	bool m_moveUp;
	bool m_moveDown;

	float m_damagesMultiplicator;
	float m_projectileSpeedMultiplicator;
	float m_hitrateMultiplicator;
};