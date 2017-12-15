#pragma once

#include "Enemy.h"

class CakeKing : public Enemy
{
	const std::string	__CAKEKING_TEXTURE = "CakeKing_Front";
	const float			__CAKEKING_DAMAGES = 5;
	const float			__CAKEKING_MAX_DAMAGES = 25;
	const float			__CAKEKING_LIFE = 1000;
	const float			__CAKEKING_MAX_LIFE = 8000;
	const float			__CAKEKING_SPEED = 200;
	const float			__CAKEKING_DASH_SPEED = 700;
	const float			__CAKEKING_DASH_MIN_DISTANCE = 400;

	const float			__CAKEKING_LIFE_INCREMENTATION_COEFFICIENT = 1.053361036;
	const float			__CAKEKING_DAMAGES_INCREMENTATION_COEFFICIENT = 1.04105638;

	const float			__CAKEKING_SPECIAL_ABILITY_COOLDOWN = 0.5f;

	const float			__CAKEKING_SPECIAL_ATTACK_COOLDOWN = 1.5f;
	const float			__CAKEKING_PROJECTILE_SPEED = 625;

public:
	explicit CakeKing(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~CakeKing() {}

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