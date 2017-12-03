#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CROCODILE_TEXTURE);

	m_velocity = __CROCODILE_SPEED;
	m_maxLife = __CROCODILE_LIFE;
	m_damages = __CROCODILE_DAMAGES;

	m_life = m_maxLife;

	m_followTarget = false;
}

Crocodile::~Crocodile()
{
}

void Crocodile::Update(const sf::Time & l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_target)
	{
		// TODO jump
	}

	Enemy::Update(l_time);
}
