#include "CakeMonster.h"
#include "StateManager.h"

CakeMonster::CakeMonster(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEMONSTER_TEXTURE);

	m_velocity = __CAKEMONSTER_SPEED;
	m_maxLife = __CAKEMONSTER_LIFE;
	m_damages = __CAKEMONSTER_DAMAGES;
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

	if (m_timer >= m_cooldown)
	{
		Shoot(); //TODO Rafale
		m_timer = 0;
	}

	Enemy::Update(l_time);
}

void CakeMonster::Shoot()
{
}
