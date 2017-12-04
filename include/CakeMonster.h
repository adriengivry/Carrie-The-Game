#pragma once

#include "Enemy.h"

class CakeMonster : public Enemy
{
	const std::string	__CAKEMONSTER_TEXTURE			= "CakeMonsterFront";
	const float			__CAKEMONSTER_SPEED				= 300;
	const float			__CAKEMONSTER_LIFE				= 10;
	const float			__CAKEMONSTER_DAMAGES			= 4;
	const float			__CAKEMONSTER_MAX_LIFE			= 150;
	const float			__CAKEMONSTER_MAX_DAMAGES		= 33;

	const float			__CAKEMONSTER_LIFE_INCREMENTATION_COEFFICIENT = 1.1f;
	const float			__CAKEMONSTER_DAMAGES_INCREMENTATION_COEFFICIENT = 1.05f;

	const float			__CAKEMONSTER_SPECIAL_ATTACK_COOLDOWN	= 0.02;
	const float			__CAKEMONSTER_PROJECTILE_SPEED			= 300;

public:
	explicit CakeMonster(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~CakeMonster();

	void GenerateStats() override;

	void SpecialAttack(const sf::Time& l_time) override;

private:
	float m_specialAttackDamages;
};