#pragma once
#include "Buyable.h"

class ExtraLife : public Buyable
{
public:
	explicit ExtraLife(SharedContext* p_sharedContext, const float p_x, const float p_y);
	~ExtraLife() {}

	void UpdatePrice() override;

	void ApplyEffect() override;
};