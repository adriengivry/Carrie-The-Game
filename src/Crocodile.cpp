#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CROCODILE_TEXTURE);
	m_velocity = __CROCODILE_SPEED;

	m_maxLife = __CROCODILE_LIFE;
	m_life = m_maxLife;
	m_damages = __CROCODILE_DAMAGES;
}

Crocodile::~Crocodile()
{
}