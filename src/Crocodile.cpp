#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CROCODILE_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity = __CROCODILE_SPEED;
	m_cooldown = __CROCODILE_COOLDOWN;

	if (m_maxLife > 350)
		m_maxLife = 350;
	else
		m_maxLife = __CROCODILE_LIFE * level * 1.1f;

	if (m_damages > 33)
		m_damages = 33;
	else
		m_damages = __CROCODILE_DAMAGES * level * 1.05;

	m_life = m_maxLife;
	m_timer = 0;
}

Crocodile::~Crocodile() {}

void Crocodile::Update(const sf::Time & l_time)
{
	SetTexture(__CROCODILE_TEXTURE);
	m_velocity = __CROCODILE_SPEED;
	m_direction.Set(0, 0);

	Enemy::Update(l_time);
}

void Crocodile::Attack()
{
	bool inRange = false;

	float range = m_position.DistanceTo(m_target->GetPosition());

	if (range < 400)
		inRange = true;
	else
		inRange = false;

	if (inRange)
		Jump();

	Enemy::Attack();
}

void Crocodile::Jump()
{
	SetTexture("CrocodileFrontRed");

	m_velocity *= 10;

	m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
	m_direction.Normalize();
}
