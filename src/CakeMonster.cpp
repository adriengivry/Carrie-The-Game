#include "CakeMonster.h"
#include "StateManager.h"

CakeMonster::CakeMonster(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEMONSTER_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity = __CAKEMONSTER_SPEED;

	if (m_maxLife > 50)
		m_maxLife = 50;
	else
		m_maxLife = __CAKEMONSTER_LIFE * (level + 0.5f);

	if (m_damages > 101)
		m_damages = 101;
	else
		m_damages = __CAKEMONSTER_DAMAGES * level * 1.05f;

	m_cooldown = __CAKEMONSTER_COOLDOWN;

	m_life = m_maxLife;
	m_timer = 0;

	m_followTarget = false;
	m_damagesOnContact = false;
}

CakeMonster::~CakeMonster()
{
}

void CakeMonster::Update(const sf::Time & l_time)
{
	m_timer += l_time.asSeconds();
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	m_isReady = m_timer >= m_cooldown;

	if (m_isReady)
	{
		Shoot(); //TODO Rafale
		m_timer = 0;
	}

	Enemy::Update(l_time);
}

void CakeMonster::Shoot()
{
	m_isReady = false;
}
