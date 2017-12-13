#pragma once
#include "Buyable.h"

class SmallCuringPotion : public Buyable
{
public:
	explicit SmallCuringPotion(SharedContext* p_sharedContext, const float p_x, const float p_y);
	~SmallCuringPotion() {}

	void UpdatePrice() override;

	void ApplyEffect() override;
};