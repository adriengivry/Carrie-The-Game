#include "Jelly.h"
#include "StateManager.h"

Jelly::Jelly(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLY_TEXTURE);

	m_velocity	= __JELLY_SPEED;
	m_maxLife	= __JELLY_LIFE;
	m_damages	= __JELLY_DAMAGES;
	m_cooldown = __JELLY_COOLDOWN;

	m_life		=	m_maxLife;
	m_timer		= 0;
}

Jelly::~Jelly() {}

void Jelly::Update(const sf::Time & l_time)
{
	m_timer += l_time.asSeconds();
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_timer >= m_cooldown)
	{
		if (m_damagesOnContact && this->IsIntersecting(player) && !player->IsInvulnerable())
		{
			player->RemoveLife(m_damages);
			player->MakeInvulnerable();
		}
		m_timer = 0;
	}

	Enemy::Update(l_time);
}
