#include "ExtraLife.h"
#include "StateManager.h"

ExtraLife::ExtraLife(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Buyable(p_sharedContext, p_x, p_y)
{
	SetTexture("Heart");
	m_itemInfo.m_name = "Extra Life";
	m_itemInfo.m_description = "+25% HP";
	m_itemInfo.m_defaultPrice = 50;
}

void ExtraLife::UpdatePrice()
{
	m_itemInfo.m_price = m_itemInfo.m_defaultPrice + m_sharedContext->m_gameInfo->m_extraLifeBought * 25;
}

void ExtraLife::ApplyEffect()
{
	m_sharedContext->m_actorManager->GetPlayer()->AddLife(25);

	++m_sharedContext->m_gameInfo->m_extraLifeBought;
}
