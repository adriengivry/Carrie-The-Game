#include "JellyBear.h"
#include "StateManager.h"

JellyBear::JellyBear(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYBEAR_TEXTURE);

	const float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity = __JELLYBEAR_SPEED;
	
	if (m_maxLife > 650)
		m_maxLife = 650;
	else
		m_maxLife = __JELLYBEAR_LIFE * level * 1.1;

	if (m_damages > 8)
		m_damages = 8;
	else
		m_damages = __JELLYBEAR_DAMAGES * level * 1.05f;

	m_cooldown = __JELLYBEAR_COOLDOWN;

	m_life = m_maxLife;
}

JellyBear::~JellyBear() {}

void JellyBear::Update(const sf::Time & l_time)
{
	TryToIntercept();

	Enemy::Update(l_time);
}

void JellyBear::TryToIntercept()
{
	m_velocity = __JELLYBEAR_SPEED;

	bool projectileFound = false;

	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		const float angle = m_position.AngleTo(it->GetPosition());

		if (angle < 5 && m_position.DistanceTo(it->GetPosition()) <= 250 && !it->MustDie())
		{
			projectileFound = true;
			SetTarget(it);
		}
		else
			SetTarget(m_sharedContext->m_actorManager->GetPlayer());
	}

	if (projectileFound)
		m_velocity *= 5;
}