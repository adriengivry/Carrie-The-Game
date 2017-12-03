#include "Jelly.h"
#include "StateManager.h"

Jelly::Jelly(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLY_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity	= __JELLY_SPEED;

	if (m_maxLife > 439)
		m_maxLife = 439;
	else
		m_maxLife = __JELLY_LIFE * level * 1.05f;

	if (m_damages > 20)
		m_damages = 20;
	else
		m_damages = __JELLY_DAMAGES * level * 1.05f;

	m_cooldown = __JELLY_COOLDOWN;

	m_life		=	m_maxLife;
	m_timer		= 0;
}

Jelly::~Jelly() {}