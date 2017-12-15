#pragma once

#include "Enemy.h"

class JellyBear : public Enemy
{
	const std::string	__JELLYBEAR_TEXTURE				= "JellyBear_Front";
	const float			__JELLYBEAR_SPEED				= 200;
	const float			__JELLYBEAR_DASH_SPEED			= 750;
	const float			__JELLYBEAR_DASH_MIN_DISTANCE	= 250;
	const float			__JELLYBEAR_LIFE				= 40;
	const float			__JELLYBEAR_MAX_LIFE			= 1200;
	const float			__JELLYBEAR_DAMAGES				= 2;
	const float			__JELLYBEAR_MAX_DAMAGES			= 10;

	const float			__JELLYBEAR_SPECIAL_ABILITY_COOLDOWN = 0.5f;

	const float			__JELLYBEAR_LIFE_INCREMENTATION_COEFFICIENT		= 1.088749657;
	const float			__JELLYBEAR_DAMAGES_INCREMENTATION_COEFFICIENT	= 1.04105638;

public:
	explicit JellyBear(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~JellyBear() {}

	void OnDeath() override;

	void GenerateStats() override;

	void Update(const sf::Time& l_time) override;

	void SpecialAbility(const sf::Time& l_time) override;

private:
	bool m_isDashing;
	float m_dashTimer;
	float m_dashMaxDuration;
};