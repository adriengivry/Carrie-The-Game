#pragma once

#include "Actor.h"

#define GENERATE_LIFE(base_life, life_inc, max)\
	m_maxLife = m_maxLife > max	? max : base_life * m_sharedContext->m_gameInfo->m_currentLevel * life_inc;

#define GENERATE_DAMAGES(base_damages, damages_inc, max)\
	m_damages = m_damages > max	? max : base_damages * m_sharedContext->m_gameInfo->m_currentLevel * damages_inc;

class Enemy : public Actor
{
	const float __ENEMY_DEFAULT_COOLDOWN = 0.0f;
public:
	explicit Enemy(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Enemy();

	virtual void GenerateStats() = 0;
	virtual void SpecialAttack(const sf::Time& l_time);
	virtual void SpecialAbility(const sf::Time& l_time);
	virtual void Attack();

	void SetTarget(Actor* p_target);

	void Kill();

	bool IsDead() const;

	void RemoveLife(const float p_damages, Vector2D<float> p_pushDirection);

	void ResetLife();

	void Update(const sf::Time& l_time) override;

	void StartSpecialAttackCooldown();
	void StartSpecialAbilityCooldown();

	void FollowTarget();

	void CheckCollisions();
	void CheckSepcialAttackCooldown(const sf::Time& l_time);
	void CheckSpecialAbilityCooldown(const sf::Time& l_time);

	void Draw() const override;
	void DrawLifebar() const;
	void DrawCooldownBar() const;

protected:
	Actor* m_target;
	bool m_followTarget;
	bool m_damagesOnContact;

	bool m_showLifeBar;
	bool m_showCooldownBar;

	bool m_isDead;
	float m_deathTimer;
	float m_deathDuration;

	bool m_canCollide;

	float m_damages;

	float m_maxLife;
	float m_life;

	bool m_isSpecialAttackReady;
	float m_specialAttackTimer;
	float m_specialAttackCooldown;

	bool m_isSpecialAbilityReady;
	float m_specialAbilityTimer;
	float m_specialAbilityCooldown;

	float m_pushMaxVelocity;
	float m_pushVelocity;
	float m_pushAcceleration;
	Vector2D<float> m_pushDirection;
};