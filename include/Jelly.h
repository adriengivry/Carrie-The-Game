#pragma once

#include "Enemy.h"

class Jelly : public Enemy
{
	const std::string	__JELLY_TEXTURE		= "Jelly_Front";
	const float			__JELLY_SPEED		= 100;
	const float			__JELLY_LIFE		= 30;
	const float			__JELLY_DAMAGES		= 2;
	const float			__JELLY_MAX_LIFE	= 450;
	const float			__JELLY_MAX_DAMAGES = 8;

	const float			__JELLY_LIFE_INCREMENTATION_COEFFICIENT		= 1.1f;
	const float			__JELLY_DAMAGES_INCREMENTATION_COEFFICIENT	= 1.05f;

public:
	explicit Jelly(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Jelly();

	void GenerateStats() override;
};