#pragma once

#include "Enemy.h"

class Boss : public Enemy
{
	const std::string	__BOSS_TEXTURE = "Boss_Front";
	const float			__BOSS_DAMAGES = 5;
	const float			__BOSS_MAX_DAMAGES = 25;
	const float			__BOSS_LIFE = 500;
	const float			__BOSS_MAX_LIFE = 1500;
	const float			__BOSS_SPEED = 400;
	const float			__BOSS_DASH_SPEED = 900;
	const float			__BOSS_DASH_MIN_DISTANCE = 400;

	const float			__BOSS_LIFE_INCREMENTATION_COEFFICIENT = 1.056467309f;
	const float			__BOSS_DAMAGES_INCREMENTATION_COEFFICIENT = 1.083798387f;

	const float			__BOSS_SPECIAL_ABILITY_COOLDOWN = 0.5f;

	const float			__BOSS_SPECIAL_ATTACK_COOLDOWN = 1.2f;
	const float			__BOSS_PROJECTILE_SPEED = 750;

public:
	explicit Boss(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Boss() {}

	void OnDeath() override;

	void GenerateStats() override;

	void Update(const sf::Time& l_time) override;

	void SpecialAttack(const sf::Time& l_time) override;
	void SpecialAbility(const sf::Time& l_time) override;

private:
	bool m_isDashing;
	float m_dashTimer;
	float m_dashMaxDuration;
};