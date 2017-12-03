#include "CakeMonster.h"
#include "StateManager.h"

CakeMonster::CakeMonster(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEMONSTER_TEXTURE);

	m_velocity = __CAKEMONSTER_SPEED;
	m_maxLife = __CAKEMONSTER_LIFE;
	m_damages = __CAKEMONSTER_DAMAGES;

	m_life = m_maxLife;

	m_followTarget = false;
	m_damagesOnContact = false;
}

CakeMonster::~CakeMonster()
{
}

void CakeMonster::Update(const sf::Time & l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_target)
	{
		// TODO Turret mode with projectile
	}

	Enemy::Update(l_time);
}
