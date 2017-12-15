#pragma once
#include "Buyable.h"

class TripleChocolateUpgrade : public Buyable
{
public:
	explicit TripleChocolateUpgrade(SharedContext* p_sharedContext, const float p_x, const float p_y);
	~TripleChocolateUpgrade() {}

	void UpdatePrice() override;

	void ApplyEffect() override;
};