#pragma once
#include "Buyable.h"

class CuringPotion : public Buyable
{
public:
	explicit CuringPotion(SharedContext* p_sharedContext, const float p_x, const float p_y);
	~CuringPotion() {}

	void UpdatePrice() override;

	void ApplyEffect() override;
};