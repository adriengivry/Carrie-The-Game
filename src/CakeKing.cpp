#include "CakeKing.h"
#include "StateManager.h"

CakeKing::CakeKing(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CAKEKING_TEXTURE);

	m_spriteScale.Set(1.8f, 1.8f);

	m_showLifeBar = true;

	CakeKing::GenerateStats();
	ResetLife();

	m_isDashing = false;
	m_dashTimer = 0.0f;
	m_dashMaxDuration = 0.2f;
}

void CakeKing::OnDeath()
{
	m_sharedContext->m_audioManager->PlaySound("Death_Cakemonster");
	m_sharedContext->m_gameInfo->m_toothPaste += 20;
	m_sharedContext->m_gameInfo->m_carrots += 20;
	m_sharedContext->m_gameInfo->m_bossBeaten = true;
}

void CakeKing::GenerateStats()
{
	m_damages = CalculateStat(__CAKEKING_DAMAGES, __CAKEKING_DAMAGES_INCREMENTATION_COEFFICIENT, __CAKEKING_MAX_DAMAGES);
	m_maxLife = CalculateStat(__CAKEKING_LIFE, __CAKEKING_LIFE_INCREMENTATION_COEFFICIENT, __CAKEKING_MAX_LIFE);

	m_velocity = __CAKEKING_SPEED;
	m_maxVelocity = m_velocity;

	m_specialAbilityCooldown = __CAKEKING_SPECIAL_ABILITY_COOLDOWN;
	m_specialAttackCooldown = __CAKEKING_SPECIAL_ATTACK_COOLDOWN;
}

void CakeKing::SpawnParticle()
{
	for (uint8_t i = 0; i < 25; ++i)
	{
		const float particleSize = Utils::randomgen(5, 10);
		const float xOffset = Utils::randomgen(0, 100) - 50;
		const float yOffset = Utils::randomgen(0, 50) - 25;
		const float angle = Utils::randomgen(0, 360);
		const uint8_t r = 255;
		const uint8_t g = 255;
		const uint8_t b = 255;
		const uint8_t a = Utils::randomgen(50, 100);
		m_sharedContext->m_actorManager->AddParticle(new Particle(m_sharedContext, m_position.X() + xOffset, m_position.Y() + yOffset, particleSize, particleSize, angle, sf::Color(r, g, b, a), 1.f));
		m_particleSpawnTimer = 0;
	}
}

void CakeKing::Update(const sf::Time& l_time)
{
	if (m_isDashing)
	{
		m_followTarget = false;
		m_specialAbilityTimer = 0.0f;
		m_velocity = __CAKEKING_DASH_SPEED;
		m_dashTimer += l_time.asSeconds();
		if (m_dashTimer >= m_dashMaxDuration)
			m_isDashing = false;
	}
	else
	{
		m_followTarget = true;
		m_velocity = __CAKEKING_SPEED;
	}

	Enemy::Update(l_time);
}

void CakeKing::SpecialAttack(const sf::Time& l_time)
{	
	if (m_lifetimeCounter >= 1.5f)
	{
		for (int i = Utils::randomgen(0, 29); i <= 360; i += Utils::randomgen(30, 59))
		{
			Projectile* projectile = new Projectile(m_sharedContext, Vector2D<float>(1, i, POLAR), this, m_position.X(), m_position.Y(), ProjectileType::CREAM, "Cream", 1.2f, 1.2f);
			projectile->SetDamages(m_damages);
			projectile->SetSpeed(__CAKEKING_PROJECTILE_SPEED);
			projectile->SetConstantlyRotate(true);
			m_sharedContext->m_actorManager->AddProjectile(projectile);
		}

		StartSpecialAttackCooldown();
	}
}

void CakeKing::SpecialAbility(const sf::Time& l_time)
{
	if (m_lifetimeCounter >= 1.5f)
	{
		if (m_isDashing)
			return;

		for (auto it : m_sharedContext->m_actorManager->GetProjectile())
		{
			if (it->IsFriendly() &&
				!it->MustDie() &&
				m_position.DistanceTo(it->GetPosition()) <= __CAKEKING_DASH_MIN_DISTANCE)
			{
				m_velocity = __CAKEKING_DASH_SPEED;
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
}
