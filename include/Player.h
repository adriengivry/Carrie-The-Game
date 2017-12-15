#pragma once

#include "Actor.h"
#include "Projectile.h"

class Player : public Actor
{
	const std::string	__PLAYER_TEXTURE		= "Carrie_Back";
	const float			__PLAYER_SPEED			= 700;
	const float			__PLAYER_ACCELERATION	= 5000;
	const float			__GETHIT_DURATION		= 1.f;

public:
	explicit Player(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Player();

	void Move(const sf::Time& l_time) override;
	void CheckFire();
	void Update(const sf::Time& l_time) override;
	void StopControl();
	void Draw() const override;

	void SpawnParticle() override;

	float GetMaxLife() const;
	float GetLife() const;

	void MakeInvulnerable();
	bool IsInvulnerable() const;

	void CheckControls();

	void Fire(EventDetails* l_details);
	void Unfire(EventDetails* l_details);
	void UseSpecialAbility(EventDetails* l_details);
	void ReleaseSpecialAbility(EventDetails* l_details);

	void RemoveLife(const float p_value, const bool p_constantDamages = false, const Vector2D<float> p_direction = Vector2D<float>(0, 0));
	void AddLife(const float p_value) const;

private:
	bool m_moveLeft;
	bool m_moveRight;
	bool m_moveUp;
	bool m_moveDown;

	bool m_collide;

	float& m_maxLife;
	float& m_life;

	bool m_fireOn;
	float m_fireTimer;
	float m_fireCooldown;

	float m_reorienteTimer;

	float m_xScaleIncrement;
	float m_yScaleIncrement;

	bool m_getHit;
	bool m_invulnerable;
	float m_timeSinceLastHit;
	float m_getHitFlashTimer;
	bool m_isTransparent;

	bool m_usingSpecialAbility;
};