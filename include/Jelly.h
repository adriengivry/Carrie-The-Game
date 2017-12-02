#pragma once

#include "Enemy.h"

class Jelly : public Enemy
{
public:
	explicit Jelly(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Jelly();

};