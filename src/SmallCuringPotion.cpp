#include "SmallCuringPotion.h"

SmallCuringPotion::SmallCuringPotion(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Buyable(p_sharedContext, p_x, p_y)
{
	SetTexture("Small_Curing_Potion");
	m_itemInfo.m_name = "Small Curing Potion";
	m_itemInfo.m_description = "Cure a random curse";
	m_itemInfo.m_defaultPrice = 50;
}


void SmallCuringPotion::UpdatePrice()
{
	m_itemInfo.m_price = m_itemInfo.m_defaultPrice + m_sharedContext->m_gameInfo->m_smallCuringPotionBought * 25;
}

void SmallCuringPotion::ApplyEffect()
{
	GameInfo* gameInfo = m_sharedContext->m_gameInfo;

	if (!gameInfo->m_reverseMovement &&
		!gameInfo->m_slowerCarrie &&
		!gameInfo->m_slowerProjectiles &&
		!gameInfo->m_weakerProjectiles &&
		!gameInfo->m_reducedPrecision)
	{
		return;
	}

	bool curseFound = false;

	do
	{
		const uint8_t selectedCurse = Utils::randomgen(0, 4);

		switch (selectedCurse)
		{
		default:
			break;

		case 0:
			curseFound = gameInfo->m_reverseMovement;
			if (curseFound)
				gameInfo->m_reverseMovement = false;
			break;

		case 1:
			curseFound = gameInfo->m_slowerCarrie;
			if (curseFound)
				gameInfo->m_slowerCarrie = 0;
			break;

		case 2:
			curseFound = gameInfo->m_slowerProjectiles;
			if (curseFound)
				gameInfo->m_slowerProjectiles = 0;
			break;

		case 3:
			curseFound = gameInfo->m_weakerProjectiles;
			if (curseFound)
				gameInfo->m_weakerProjectiles = 0;
			break;

		case 4:
			curseFound = gameInfo->m_reducedPrecision;
			if (curseFound)
				gameInfo->m_reducedPrecision = 0;
			break;
		}
	} while (!curseFound);

	++m_sharedContext->m_gameInfo->m_smallCuringPotionBought;
}
