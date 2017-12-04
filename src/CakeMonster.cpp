#include "CakeMonster.h"
#include "StateManager.h"

CakeMonster::CakeMonster(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEMONSTER_TEXTURE);

	CakeMonster::GenerateStats();
	ResetLife();

	m_followTarget = false;
}

CakeMonster::~CakeMonster() {}

void CakeMonster::GenerateStats()
{
	m_velocity = __CAKEMONSTER_SPEED;
	m_specialAttackCooldown = __CAKEMONSTER_SPECIAL_ATTACK_COOLDOWN;

	GENERATE_LIFE(__CAKEMONSTER_LIFE, __CAKEMONSTER_LIFE_INCREMENTATION_COEFFICIENT, __CAKEMONSTER_MAX_LIFE);
	GENERATE_DAMAGES(__CAKEMONSTER_DAMAGES, __CAKEMONSTER_DAMAGES_INCREMENTATION_COEFFICIENT, __CAKEMONSTER_MAX_DAMAGES);

	m_specialAttackDamages = m_damages / 15.0f;
}

void CakeMonster::SpecialAttack(const sf::Time& l_time)
{
	Vector2D<float> projectileDirection;
	projectileDirection.Set(1, m_position.AngleTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()), POLAR);

	Projectile* projectile = new Projectile(m_sharedContext, projectileDirection, m_position.X(), m_position.Y(), false, true);

	// DAMAGES PER SECOND
	projectile->SetDamages(m_specialAttackDamages);
	projectile->SetSpeed(__CAKEMONSTER_PROJECTILE_SPEED);

	m_sharedContext->m_actorManager->AddProjectile(projectile);

	StartSpecialAttackCooldown();
}
