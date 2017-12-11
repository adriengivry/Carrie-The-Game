#include "CakeMonster.h"
#include "StateManager.h"

CakeMonster::CakeMonster(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEMONSTER_TEXTURE);

	CakeMonster::GenerateStats();
	ResetLife();

	m_followTarget = false;

	m_attackState = AttackState::ATTACK;

	m_fireTimer = 0;
	m_reloadTimer = 0;

	m_shadowOffset = 50;
}

void CakeMonster::OnDeath()
{
	m_sharedContext->m_audioManager->PlaySound("Death_Cakemonster");
	m_sharedContext->m_gameInfo->m_toothPaste += 5;
}

void CakeMonster::GenerateStats()
{
	m_velocity = __CAKEMONSTER_SPEED;
	m_maxVelocity = m_velocity;

	m_specialAttackCooldown = __CAKEMONSTER_SPECIAL_ATTACK_COOLDOWN;

	m_maxLife = CalculateStat(__CAKEMONSTER_LIFE, __CAKEMONSTER_LIFE_INCREMENTATION_COEFFICIENT, __CAKEMONSTER_MAX_LIFE);
	m_damages = CalculateStat(__CAKEMONSTER_DAMAGES, __CAKEMONSTER_DAMAGES_INCREMENTATION_COEFFICIENT, __CAKEMONSTER_MAX_DAMAGES);

	m_specialAttackDamages = m_damages / 15.0f;
}

void CakeMonster::SpecialAttack(const sf::Time& l_time)
{
	if (m_attackState == AttackState::ATTACK)
	{
		Vector2D<float> projectileDirection;
		projectileDirection.Set(1, m_position.AngleTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()), POLAR);
		Projectile* projectile = new Projectile(m_sharedContext, projectileDirection, this, m_position.X(), m_position.Y(), ProjectileType::LASER_CREAM, "Cream", 1.f, 1.f);
		projectile->SetDamages(m_specialAttackDamages);
		projectile->SetSpeed(__CAKEMONSTER_PROJECTILE_SPEED);
		projectile->SetConstantlyRotate(true);
		m_sharedContext->m_actorManager->AddProjectile(projectile);
		StartSpecialAttackCooldown();
	}
}

void CakeMonster::Update(const sf::Time& l_time)
{
	Enemy::Update(l_time);

	switch (m_attackState)
	{
	case AttackState::RELOAD:
		m_reloadTimer += l_time.asSeconds();
		if (m_reloadTimer >= __CAKEMONSTER_RELOAD_DURATION)
		{
			m_attackState = AttackState::ATTACK;
			m_fireTimer = 0;
		}
		break;

	case AttackState::ATTACK:
		m_fireTimer += l_time.asSeconds();	

		if (m_fireTimer >= __CAKEMONSTER_FIRE_DURATION)
		{
			m_attackState = AttackState::RELOAD;
			m_reloadTimer = 0;
		}
		break;
	}

}