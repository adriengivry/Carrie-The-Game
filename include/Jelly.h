#pragma once

#include "Enemy.h"

class Jelly : public Enemy
{
	const std::string	__JELLY_TEXTURE = "JellyFront";
	const float			__JELLY_SPEED = 100;
	const float			__JELLY_LIFE = 6;
	const float			__JELLY_DAMAGES = 2;

public:
	explicit Jelly(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Jelly();

};