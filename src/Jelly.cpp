#include "Jelly.h"
#include "StateManager.h"

Jelly::Jelly(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLY_TEXTURE);

	m_velocity	= __JELLY_SPEED;
	m_maxLife	= __JELLY_LIFE;
	m_damages	= __JELLY_DAMAGES;

	m_life		=	m_maxLife;
}

Jelly::~Jelly() {}
