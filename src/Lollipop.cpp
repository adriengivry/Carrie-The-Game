#include "Lollipop.h"
#include "StateManager.h"

Lollipop::Lollipop(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__LOLLIPOP_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity	= __LOLLIPOP_SPEED;

	if (m_maxLife > 50)
		m_maxLife = 50;
	else
		m_maxLife	= __LOLLIPOP_LIFE * (level + 0.5f);

	if (m_damages > 52)
		m_damages = 52;
	else
		m_damages = __LOLLIPOP_DAMAGES * level * 1.03f;

	m_cooldown	= __LOLLIPOP_COOLDOWN;

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
	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (m_position.DistanceTo(it->GetPosition()) <= 400 && !it->MustDie())
		{
			const float angle = m_position.AngleTo(it->GetPosition());

			if (angle < 45)
				m_direction.Set(-it->GetPosition().Y(), it->GetPosition().X());
			else
				m_direction.Set(it->GetPosition().Y(), -it->GetPosition().X());

			m_direction.Normalize();
		}
		else
		{
			m_direction.Set(0, 0);
		}
	}
}

void Lollipop::Shoot()
{
	//TODO
}
