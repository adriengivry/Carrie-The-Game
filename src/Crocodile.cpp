#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CROCODILE_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity = __CROCODILE_SPEED;

	if (m_maxLife > 230)
		m_maxLife = 230;
	else
		m_maxLife = __CROCODILE_LIFE * level * 1.15f;

	if (m_damages > 101)
		m_damages = 101;
	else
		m_damages = __CROCODILE_DAMAGES * level * 1.05;

	m_cooldown = __CROCODILE_COOLDOWN;

	m_life = m_maxLife;
	m_timer = 0;

	m_followTarget = false;
}

Crocodile::~Crocodile() {}

void Crocodile::Update(const sf::Time & l_time)
{
	Jump();

	Enemy::Update(l_time);
}

void Crocodile::Jump()
{
	//TODO
}
