#include "TripleChocolateUpgrade.h"

TripleChocolateUpgrade::TripleChocolateUpgrade(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Buyable(p_sharedContext, p_x, p_y, MoneyType::CARROT)
{
	SetTexture("Triple_Chocolate");
	m_itemInfo.m_name = "Triple Chocolate";
	m_itemInfo.m_description = "3 times more chocolate!";
	m_itemInfo.m_defaultPrice = 120;

	m_owned = m_sharedContext->m_gameInfo->m_tripleChocolateUpgrade;
}


void TripleChocolateUpgrade::UpdatePrice()
{
	m_itemInfo.m_price = m_itemInfo.m_defaultPrice;
}

void TripleChocolateUpgrade::ApplyEffect()
{
	GameInfo* gameInfo = m_sharedContext->m_gameInfo;

	gameInfo->m_tripleChocolateUpgrade = true;
	m_owned = true;
}
