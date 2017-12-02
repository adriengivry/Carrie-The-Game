#include "Lollipop.h"
#include "StateManager.h"

Lollipop::Lollipop(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__LOLLIPOP_TEXTURE);
	m_velocity = __LOLLIPOP_SPEED;

	m_maxLife = __LOLLIPOP_LIFE;
	m_life = m_maxLife;
	m_damages = __LOLLIPOP_DAMAGES;

	SetTarget(nullptr);
}

Lollipop::~Lollipop()
{
}