#pragma once
#include "Buyable.h"

class PerforantChocolateUpgrade : public Buyable
{
public:
	explicit PerforantChocolateUpgrade(SharedContext* p_sharedContext, const float p_x, const float p_y);
	~PerforantChocolateUpgrade() {}

	void UpdatePrice() override;

	void ApplyEffect() override;
};