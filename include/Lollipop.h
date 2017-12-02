#pragma once

#include "Enemy.h"

class Lollipop : public Enemy
{
	const std::string	__LOLLIPOP_TEXTURE = "LollipopFront";
	const float			__LOLLIPOP_SPEED = 300;
	const float			__LOLLIPOP_LIFE = 1;
	const float			__LOLLIPOP_DAMAGES = 4;

public:
	explicit Lollipop(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Lollipop();

};