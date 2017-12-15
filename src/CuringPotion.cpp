#include "CuringPotion.h"

CuringPotion::CuringPotion(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Buyable(p_sharedContext, p_x, p_y)
{
	SetTexture("Curing_Potion");
	m_itemInfo.m_name = "Curing Potion";
	m_itemInfo.m_description = "Cure every curse";
	m_itemInfo.m_defaultPrice = 400;
}


void CuringPotion::UpdatePrice()
{
	m_itemInfo.m_price = m_itemInfo.m_defaultPrice + m_sharedContext->m_gameInfo->m_curingPotionBought * 200;
}

void CuringPotion::ApplyEffect()
{
	m_sharedContext->m_gameInfo->m_reverseMovement = false;
	m_sharedContext->m_gameInfo->m_slowerCarrie = 0;
	m_sharedContext->m_gameInfo->m_slowerProjectiles = 0;
	m_sharedContext->m_gameInfo->m_weakerProjectiles = 0;
	m_sharedContext->m_gameInfo->m_reducedPrecision = 0;

	++m_sharedContext->m_gameInfo->m_curingPotionBought;
}
