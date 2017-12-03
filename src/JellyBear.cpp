#include "JellyBear.h"
#include "StateManager.h"

JellyBear::JellyBear(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYBEAR_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity = __JELLYBEAR_SPEED;
	
	if (m_maxLife > 665)
		m_maxLife = 665;
	else
		m_maxLife = __JELLYBEAR_LIFE * level * 1.12;

	if (m_damages > 20)
		m_damages = 20;
	else
		m_damages = __JELLYBEAR_DAMAGES * level * 1.05f;

	m_cooldown = __JELLYBEAR_COOLDOWN;

	m_life = m_maxLife;
	m_timer = 0;
}

JellyBear::~JellyBear()
{
}

void JellyBear::Update(const sf::Time & l_time)
{
	m_timer += l_time.asSeconds();
	m_velocity = __JELLYBEAR_SPEED;

	Player* player = m_sharedContext->m_actorManager->GetPlayer();

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

	m_isReady = m_timer >= m_cooldown;

	if (m_isReady)
	{
		if (m_damagesOnContact && this->IsIntersecting(player) && !player->IsInvulnerable())
		{
			player->RemoveLife(m_damages);
			player->MakeInvulnerable();
			m_isReady = false;
		}
		m_timer = 0;
	}

	Enemy::Update(l_time);
}
