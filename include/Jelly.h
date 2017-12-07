#pragma once

#include "Enemy.h"

class Jelly : public Enemy
{
	const std::string	__JELLY_TEXTURE		= "Jelly_Front";
	const float			__JELLY_SPEED		= 100;
	const float			__JELLY_LIFE		= 30;
	const float			__JELLY_MAX_LIFE	= 400;
	const float			__JELLY_DAMAGES		= 2;
	const float			__JELLY_MAX_DAMAGES = 5;

	const float			__JELLY_LIFE_INCREMENTATION_COEFFICIENT		= 1.13827432f;
	const float			__JELLY_DAMAGES_INCREMENTATION_COEFFICIENT	= 1.04688023f;

public:
	explicit Jelly(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Jelly() {}

	void OnDeath() override;

	void GenerateStats() override;
};