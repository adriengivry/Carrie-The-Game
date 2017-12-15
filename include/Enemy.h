#pragma once

#include "Actor.h"

class Enemy : public Actor
{
	const float __ENEMY_DEFAULT_COOLDOWN = 0.0f;
	const float __ENEMY_MIN_BAR_SIZE = 80.f;
public:
	explicit Enemy(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Enemy();

	virtual void OnDeath() = 0;

	virtual void GenerateStats() = 0;
	virtual void SpecialAttack(const sf::Time& l_time);
	virtual void SpecialAbility(const sf::Time& l_time);
	virtual void Attack();

	float CalculateStat(const float p_baseValue, const float p_valueIncrement, const float p_maxValue) const;

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
	void DrawSpecialAttackCooldownBar() const;
	void DrawSpecialAbilityCooldownBar() const;

protected:
	Actor* m_target;
	bool m_followTarget;
	bool m_damagesOnContact;

	bool m_showLifeBar;
	bool m_showSpecialAttackBar;
	bool m_showSpecialAbilityBar;

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
};