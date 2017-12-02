#pragma once

#include "Enemy.h"

class PastryBag : public Enemy
{
	const std::string	__PASTRYBAG_TEXTURE = "PastryBag";
	const float			__PASTRYBAG_SPEED = 300;
	const float			__PASTRYBAG_LIFE = 1;
	const float			__PASTRYBAG_DAMAGES = 8;

public:
	explicit PastryBag(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~PastryBag();

};