#include "JellyBear.h"
#include "StateManager.h"

JellyBear::JellyBear(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYBEAR_TEXTURE);

	m_velocity = __JELLYBEAR_SPEED;
	m_maxLife = __JELLYBEAR_LIFE;
	m_damages = __JELLYBEAR_DAMAGES;

	m_life = m_maxLife;
}

JellyBear::~JellyBear()
{
}

void JellyBear::Update(const sf::Time & l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_target)
	{
		// TODO jump on bullet
	}

	Enemy::Update(l_time);
}
