#pragma once

#include "Enemy.h"

class Lollipop : public Enemy
{
	const std::string	__LOLLIPOP_TEXTURE				= "LollipopFront";
	const float			__LOLLIPOP_SPEED				= 80;
	const float			__LOLLIPOP_DASH_SPEED			= 80;
	const float			__LOLLIPOP_DASH_MIN_DISTANCE	= 250;
	const float			__LOLLIPOP_LIFE					= 10;
	const float			__LOLLIPOP_DAMAGES				= 4;
	const float			__LOLLIPOP_MAX_LIFE				= 150;
	const float			__LOLLIPOP_MAX_DAMAGES			= 16;

	const float			__LOLLIPOP_SPECIAL_ABILITY_COOLDOWN = 0.5f;

	const float			__LOLLIPOP_LIFE_INCREMENTATION_COEFFICIENT		= 1.1f;
	const float			__LOLLIPOP_DAMAGES_INCREMENTATION_COEFFICIENT	= 1.05f;

	const float			__LOLLIPOP_SPECIAL_ATTACK_COOLDOWN	= 1.2;
	const float			__LOLLIPOP_PROJECTILE_SPEED			= 560;

public:
	explicit Lollipop(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Lollipop();

	void GenerateStats() override;

	void Update(const sf::Time& l_time) override;

	void SpecialAttack(const sf::Time& l_time) override;
	void SpecialAbility(const sf::Time& l_time) override;

private:
	bool m_isDashing;
	float m_dashTimer;
	float m_dashMaxDuration;
};