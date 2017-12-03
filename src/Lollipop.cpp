#include "Lollipop.h"
#include "StateManager.h"

Lollipop::Lollipop(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__LOLLIPOP_TEXTURE);

	m_velocity	= __LOLLIPOP_SPEED;
	m_maxLife	= __LOLLIPOP_LIFE;
	m_damages	= __LOLLIPOP_DAMAGES;

	m_life		=	m_maxLife;

	m_followTarget		= false;
	m_damagesOnContact	= false;
}

Lollipop::~Lollipop() {}

void Lollipop::Update(const sf::Time & l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_target)
	{
		// TODO throw projectile
	}

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

	Enemy::Update(l_time);
}
