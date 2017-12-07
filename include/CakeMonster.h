#pragma once

#include "Enemy.h"

enum class AttackState
{
	ATTACK,
	RELOAD
};

class CakeMonster : public Enemy
{
	const std::string	__CAKEMONSTER_TEXTURE			= "CakeMonster_Front";
	const float			__CAKEMONSTER_SPEED				= 300;
	const float			__CAKEMONSTER_LIFE				= 10;
	const float			__CAKEMONSTER_DAMAGES			= 4;
	const float			__CAKEMONSTER_MAX_LIFE			= 150;
	const float			__CAKEMONSTER_MAX_DAMAGES		= 16;

	const float			__CAKEMONSTER_LIFE_INCREMENTATION_COEFFICIENT = 1.1f;
	const float			__CAKEMONSTER_DAMAGES_INCREMENTATION_COEFFICIENT = 1.05f;

	const float			__CAKEMONSTER_SPECIAL_ATTACK_COOLDOWN		= 0.05;
	const float			__CAKEMONSTER_PROJECTILE_SPEED				= 300;
	const float			__CAKEMONSTER_FIRE_DURATION					= 1.5f;
	const float			__CAKEMONSTER_RELOAD_DURATION				= 1.f;

public:
	explicit CakeMonster(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~CakeMonster() {}

	void OnDeath() override;

	void GenerateStats() override;

	void SpecialAttack(const sf::Time& l_time) override;

	void Update(const sf::Time& l_time) override;

private:
	float m_specialAttackDamages;

	AttackState m_attackState;
	float m_fireTimer;
	float m_reloadTimer;
};