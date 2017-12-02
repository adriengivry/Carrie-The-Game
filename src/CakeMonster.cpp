#include "CakeMonster.h"
#include "StateManager.h"

CakeMonster::CakeMonster(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEMONSTER_TEXTURE);
	m_velocity = __CAKEMONSTER_SPEED;

	m_maxLife = __CAKEMONSTER_LIFE;
	m_life = m_maxLife;
	m_damages = __CAKEMONSTER_DAMAGES;

	SetTarget(nullptr);
}

CakeMonster::~CakeMonster()
{
}