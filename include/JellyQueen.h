#pragma once

#include "Enemy.h"

class JellyQueen : public Enemy
{
	const std::string	__JELLYQUEEN_TEXTURE = "JellyQueen_Front";
	const float			__JELLYQUEEN_DAMAGES = 5;
	const float			__JELLYQUEEN_MAX_DAMAGES = 25;
	const float			__JELLYQUEEN_LIFE = 1000;
	const float			__JELLYQUEEN_MAX_LIFE = 8000;
	const float			__JELLYQUEEN_SPEED = 100;
	const float			__JELLYQUEEN_DASH_MIN_DISTANCE = 2000;
	const float			__JELLYQUEEN_DASH_SPEED = 1250;

	const float			__JELLYQUEEN_LIFE_INCREMENTATION_COEFFICIENT = 1.053361036;
	const float			__JELLYQUEEN_DAMAGES_INCREMENTATION_COEFFICIENT = 1.04105638;

	const float			__JELLYQUEEN_SPECIAL_ABILITY_COOLDOWN = 2.f;

	const float			__JELLYQUEEN_SPECIAL_ATTACK_COOLDOWN = 0.8f;
	const float			__JELLYQUEEN_PROJECTILE_SPEED = 800;

public:
	explicit JellyQueen(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~JellyQueen() {}

	void OnDeath() override;

	void GenerateStats() override;

	void SpawnParticle() override;

	void Update(const sf::Time& l_time) override;

	void SpecialAttack(const sf::Time& l_time) override;
	void SpecialAbility(const sf::Time& l_time) override;

private:
	bool m_isDashing;
	float m_dashTimer;
	float m_dashMaxDuration;
};