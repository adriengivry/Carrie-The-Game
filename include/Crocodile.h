#pragma once

#include "Enemy.h"

class Crocodile : public Enemy
{
	const std::string	__CROCODILE_TEXTURE = "CrocodileFront";
	const float			__CROCODILE_SPEED = 600;
	const float			__CROCODILE_LIFE = 4;
	const float			__CROCODILE_DAMAGES = 8;
	const float			__CROCODILE_COOLDOWN = 5;

public:
	explicit Crocodile(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Crocodile();

	void Update(const sf::Time& l_time) override;

	void Jump();

};