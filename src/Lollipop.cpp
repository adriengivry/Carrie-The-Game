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

void Lollipop::Update(const sf::Time & l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_target)
	{
		//TODO Projectile
		player->RemoveLife(m_damages);
		player->MakeInvulnerable();
	}

	Actor::Update(l_time);

	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (this->m_position.DistanceTo(it->GetPosition()) <= 400)
		{
			m_direction.Set(it->GetPosition().Y(), -it->GetPosition().X());
			m_direction.AngleTo(it->GetPosition());
			m_direction.Normalize();

			m_position += m_direction * m_velocity * l_time.asSeconds();


			m_sprite.setPosition(m_position.ToSFVector());
		}
		else
		{
			m_direction.Set(0, 0);
		}
	}

	if (m_position.X() < -100 || m_position.X() > 2000 || m_position.Y() < -100 || m_position.Y() > 1200)
		m_mustDie = true;
}
