#include "Boss.h"
#include "StateManager.h"

Boss::Boss(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__BOSS_TEXTURE);

	m_spriteScale.Set(2.f, 2.f);

	m_showLifeBar = true;

	Boss::GenerateStats();
	ResetLife();

	m_isDashing = false;
	m_dashTimer = 0.0f;
	m_dashMaxDuration = 0.2f;
}

void Boss::OnDeath()
{
}

void Boss::GenerateStats()
{
	m_damages = CalculateStat(__BOSS_DAMAGES, __BOSS_DAMAGES_INCREMENTATION_COEFFICIENT, __BOSS_MAX_DAMAGES);
	m_maxLife = CalculateStat(__BOSS_LIFE, __BOSS_LIFE_INCREMENTATION_COEFFICIENT, __BOSS_MAX_LIFE);

	m_velocity = __BOSS_SPEED;
	m_maxVelocity = m_velocity;

	m_specialAbilityCooldown = __BOSS_SPECIAL_ABILITY_COOLDOWN;
	m_specialAttackCooldown = __BOSS_SPECIAL_ATTACK_COOLDOWN;
}

void Boss::Update(const sf::Time& l_time)
{
	if (m_isDashing)
	{
		m_followTarget = false;
		m_specialAbilityTimer = 0.0f;
		m_velocity = __BOSS_DASH_SPEED;
		m_dashTimer += l_time.asSeconds();
		if (m_dashTimer >= m_dashMaxDuration)
			m_isDashing = false;
	}
	else
	{
		m_followTarget = true;
		m_velocity = __BOSS_SPEED;
	}

	Enemy::Update(l_time);
}

void Boss::SpecialAttack(const sf::Time& l_time)
{	
	for (int i = Utils::randomgen(0, 59); i <= 360; i += Utils::randomgen(30, 100))
	{
		Projectile* projectile = new Projectile(m_sharedContext, Vector2D<float>(1, i, POLAR), this, m_position.X(), m_position.Y(), ProjectileType::LASER, "Cream", 1.2f, 1.2f);
		projectile->SetDamages(m_damages);
		projectile->SetSpeed(__BOSS_PROJECTILE_SPEED);
		projectile->SetConstantlyRotate(true);
		m_sharedContext->m_actorManager->AddProjectile(projectile);
	}

	StartSpecialAttackCooldown();
}

void Boss::SpecialAbility(const sf::Time& l_time)
{
	if (m_isDashing)
		return;

	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (it->IsFriendly() &&
			!it->MustDie() &&
			m_position.DistanceTo(it->GetPosition()) <= __BOSS_DASH_MIN_DISTANCE)
		{
			m_velocity = __BOSS_DASH_SPEED;
			const uint8_t rand = Utils::randomgen(0, 1);

			if (rand)
			{
				m_direction.X() = -it->GetDirection().Y();
				m_direction.Y() = it->GetDirection().X();
			}
			else if (!rand)
			{
				m_direction.X() = it->GetDirection().Y();
				m_direction.Y() = -it->GetDirection().X();
			}

			m_direction.Normalize();
			m_isDashing = true;
			m_dashTimer = 0.0f;
			StartSpecialAbilityCooldown();
			return;
		}
	}
}
