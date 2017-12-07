#pragma once

#include "Enemy.h"

class Crocodile : public Enemy
{
	const std::string	__CROCODILE_TEXTURE_GREEN		= "Crocodile_Green_Right";
	const std::string	__CROCODILE_TEXTURE_RED			= "Crocodile_Red_Right";
	const float			__CROCODILE_SPEED				= 60;
	const float			__CROCODILE_DASH_SPEED			= 600;
	const float			__CROCODILE_DASH_MIN_DISTANCE	= 350;
	const float			__CROCODILE_LIFE				= 20;
	const float			__CROCODILE_MAX_LIFE			= 300;
	const float			__CROCODILE_DAMAGES				= 8;
	const float			__CROCODILE_MAX_DAMAGES			= 20;

	const float			__CROCODILE_LIFE_INCREMENTATION_COEFFICIENT = 1.14499756f;
	const float			__CROCODILE_DAMAGES_INCREMENTATION_COEFFICIENT = 1.04688023f;

	const float			__CROCODILE_SPECIAL_ABILITY_COOLDOWN = 5;

public:
	explicit Crocodile(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Crocodile() {}

	void OnDeath() override;

	void GenerateStats() override;

	void Update(const sf::Time& l_time) override;

	void SpecialAbility(const sf::Time& l_time) override;

private:
	bool m_isDashing;
	float m_dashTimer;
	float m_dashMaxDuration;
};