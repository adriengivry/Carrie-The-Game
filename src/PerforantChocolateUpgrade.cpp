#include "PerforantChocolateUpgrade.h"

PerforantChocolateUpgrade::PerforantChocolateUpgrade(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Buyable(p_sharedContext, p_x, p_y, MoneyType::CARROT)
{
	SetTexture("Perforant_Chocolate_Buyable");
	m_itemInfo.m_name = "Perforant Chocolate";
	m_itemInfo.m_description = "Pierce them all!";
	m_itemInfo.m_defaultPrice = 10;

	m_owned = m_sharedContext->m_gameInfo->m_perforantChocolateUpgrade;
}


void PerforantChocolateUpgrade::UpdatePrice()
{
	m_itemInfo.m_price = m_itemInfo.m_defaultPrice;
}

void PerforantChocolateUpgrade::ApplyEffect()
{
	GameInfo* gameInfo = m_sharedContext->m_gameInfo;

	gameInfo->m_perforantChocolateUpgrade = true;
	m_owned = true;
}
