#include "Lollipop.h"
#include "StateManager.h"

Lollipop::Lollipop(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__LOLLIPOP_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity	= __LOLLIPOP_SPEED;
	m_cooldown = __LOLLIPOP_COOLDOWN;

	if (m_maxLife > 150)
		m_maxLife = 150;
	else
		m_maxLife	= __LOLLIPOP_LIFE * level * 1.1f;

	if (m_damages > 16)
		m_damages = 16;
	else
		m_damages = __LOLLIPOP_DAMAGES * level * 1.05f;

	m_life		= m_maxLife;
	m_timer		= 0;

	m_followTarget		= false;
	m_damagesOnContact	= false;
}

Lollipop::~Lollipop() {}

void Lollipop::Update(const sf::Time & l_time)
{
	Dodge();

	Enemy::Update(l_time);
}

void Lollipop::Attack()
{
	Shoot();

	Enemy::Attack();
}

void Lollipop::Dodge()
{
	bool isRight = false;

	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (m_position.DistanceTo(it->GetPosition()) <= 400 && !it->MustDie())
		{
			if (it->GetPosition().X() > m_position.X())
				isRight = true;

			if (isRight)
				m_direction.Set(1, m_position.AngleTo(it->GetPosition()) + 90, AGMath::POLAR);
			else
				m_direction.Set(1, m_position.AngleTo(it->GetPosition()) - 90, AGMath::POLAR);

			m_direction.Normalize();
		}
		else
			m_direction.Set(0, 0);
	}
}

void Lollipop::Shoot()
{
	//TODO
}
