#pragma once

#include "Actor.h"
#include "Projectile.h"

class Player : public Actor
{
	const std::string	__PLAYER_TEXTURE = "Back";
	const float			__PLAYER_SPEED	 = 400;
	const float			__PLAYER_LIFE	 = 100;
	const float			__INVULNERABLE_DURATION = 0.5f;

public:
	explicit Player(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Player();

	void React(EventDetails* l_details);
	void Unreact(EventDetails* l_details);
	void Move(const sf::Time& l_time) override;
	void Update(const sf::Time& l_time) override;
	void StopControl();
	void Draw() const override;

	float GetMaxLife() const;
	float GetLife() const;

	void MakeInvulnerable();
	bool IsInvulnerable() const;

	void Fire(EventDetails* l_details);
	void RemoveLife(const float p_value, const bool p_constantDamages = false);

private:
	bool m_moveLeft;
	bool m_moveRight;
	bool m_moveUp;
	bool m_moveDown;

	bool m_collide;

	float m_maxLife;
	float m_life;

	bool m_invulnerable;
	float m_invulnerableTimer;
};