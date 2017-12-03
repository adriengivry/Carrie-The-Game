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

	if (this->IsIntersecting(player))
	{
		m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
		m_direction.Normalize();

		m_position -= m_direction * m_velocity * l_time.asSeconds();

		m_sprite.setPosition(m_position.ToSFVector());
	}

	if (m_position.X() < -100 || m_position.X() > 2000 || m_position.Y() < -100 || m_position.Y() > 1200)
		m_mustDie = true;
}
