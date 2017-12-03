#pragma once

#include "Enemy.h"

class JellyBear : public Enemy
{
	const std::string	__JELLYBEAR_TEXTURE = "JellyBearFront";
	const float			__JELLYBEAR_SPEED = 200;
	const float			__JELLYBEAR_LIFE = 8;
	const float			__JELLYBEAR_DAMAGES = 2;
	const float			__JELLYBEAR_COOLDOWN = 1.6;

public:
	explicit JellyBear(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~JellyBear();

	void Update(const sf::Time& l_time) override;

};