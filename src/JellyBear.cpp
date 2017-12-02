#include "JellyBear.h"
#include "StateManager.h"

JellyBear::JellyBear(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYBEAR_TEXTURE);
	m_velocity = __JELLYBEAR_SPEED;

	m_maxLife = __JELLYBEAR_LIFE;
	m_life = m_maxLife;
	m_damages = __JELLYBEAR_DAMAGES;
}

JellyBear::~JellyBear()
{
}
