#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CROCODILE_TEXTURE);

	m_velocity = __CROCODILE_SPEED;
	m_maxLife = __CROCODILE_LIFE;
	m_damages = __CROCODILE_DAMAGES;
	m_cooldown = __CROCODILE_COOLDOWN;

	m_life = m_maxLife;
	m_timer = 0;

	m_followTarget = false;
}

Crocodile::~Crocodile()
{
}

void Crocodile::Update(const sf::Time & l_time)
{
	m_timer += l_time.asSeconds();
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_timer >= m_cooldown)
	{
		Jump(); //TODO

		if (m_damagesOnContact && this->IsIntersecting(player) && !player->IsInvulnerable())
		{
			player->RemoveLife(m_damages);
			player->MakeInvulnerable();
		}
		m_timer = 0;
	}

	Enemy::Update(l_time);
}

void Crocodile::Jump()
{
}
