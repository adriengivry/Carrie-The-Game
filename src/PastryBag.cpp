#include "PastryBag.h"
#include "StateManager.h"

PastryBag::PastryBag(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__PASTRYBAG_TEXTURE);
	m_velocity = __PASTRYBAG_SPEED;

	m_maxLife = __PASTRYBAG_LIFE;
	m_life = m_maxLife;
	m_damages = __PASTRYBAG_DAMAGES;
}

PastryBag::~PastryBag()
{
}