#include "JellyQueen.h"
#include "StateManager.h"

JellyQueen::JellyQueen(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYQUEEN_TEXTURE);

	m_spriteScale.Set(1.8f, 1.8f);

	m_showLifeBar = true;

	JellyQueen::GenerateStats();
	ResetLife();

	m_isDashing = false;
	m_dashTimer = 0.0f;
	m_dashMaxDuration = 0.4;
}

void JellyQueen::OnDeath()
{
	m_sharedContext->m_audioManager->PlaySound("Death_Jelly");
	m_sharedContext->m_gameInfo->m_toothPaste += 20;
	m_sharedContext->m_gameInfo->m_bossBeaten = true;
}

void JellyQueen::GenerateStats()
{
	m_damages = CalculateStat(__JELLYQUEEN_DAMAGES, __JELLYQUEEN_DAMAGES_INCREMENTATION_COEFFICIENT, __JELLYQUEEN_MAX_DAMAGES);
	m_maxLife = CalculateStat(__JELLYQUEEN_LIFE, __JELLYQUEEN_LIFE_INCREMENTATION_COEFFICIENT, __JELLYQUEEN_MAX_LIFE);

	m_velocity = __JELLYQUEEN_SPEED;
	m_maxVelocity = m_velocity;

	m_specialAbilityCooldown = __JELLYQUEEN_SPECIAL_ABILITY_COOLDOWN;
	m_specialAttackCooldown = __JELLYQUEEN_SPECIAL_ATTACK_COOLDOWN;
}

void JellyQueen::SpawnParticle()
{
	for (uint8_t i = 0; i < 25; ++i)
	{
		const float particleSize = Utils::randomgen(5, 10);
		const float xOffset = Utils::randomgen(0, 50) - 25;
		const float yOffset = Utils::randomgen(0, 100) - 50;
		const float angle = Utils::randomgen(0, 360);
		const uint8_t r = Utils::randomgen(160, 185);
		const uint8_t g = Utils::randomgen(35, 54);
		const uint8_t b = Utils::randomgen(80, 100);
		const uint8_t a = 80;
		m_sharedContext->m_actorManager->AddParticle(new Particle(m_sharedContext, m_position.X() + xOffset, m_position.Y() + m_shadowOffset + yOffset, particleSize, particleSize, angle, sf::Color(r, g, b, a), 2.f));
		m_particleSpawnTimer = 0;
	}
}

void JellyQueen::Update(const sf::Time& l_time)
{
	if (m_isDashing)
	{
		m_specialAbilityTimer = 0.0f;
		m_velocity = __JELLYQUEEN_DASH_SPEED;
		m_dashTimer += l_time.asSeconds();
		if (m_dashTimer >= m_dashMaxDuration)
			m_isDashing = false;
	}
	else
	{
		m_velocity = __JELLYQUEEN_SPEED;
		m_followTarget = true;
	}

	Enemy::Update(l_time);
}

void JellyQueen::SpecialAttack(const sf::Time& l_time)
{
	if (m_lifetimeCounter >= 1.5f)
	{
		Vector2D<float> projectileDirection;
		projectileDirection.Set(1, m_position.AngleTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()) + Utils::randomgen(0, 60) - 30, POLAR);
		Projectile* projectile = new Projectile(m_sharedContext, projectileDirection, this, m_position.X(), m_position.Y(), ProjectileType::STRONG, "Cupcake", 1.4f, 1.4f);
		projectile->SetSpeed(__JELLYQUEEN_PROJECTILE_SPEED);
		projectile->SetDamages(m_damages);
		projectile->SetConstantlyRotate(true);
		projectile->SetColor(sf::Color(Utils::randomgen(0, 255), Utils::randomgen(0, 255), Utils::randomgen(0, 255)));

		m_sharedContext->m_actorManager->AddProjectile(projectile);

		StartSpecialAttackCooldown();
	}
}

void JellyQueen::SpecialAbility(const sf::Time& l_time)
{
	if (m_lifetimeCounter >= 1.5f)
	{
		if (m_position.DistanceTo(m_target->GetPosition()) < __JELLYQUEEN_DASH_MIN_DISTANCE)
		{
			m_velocity = __JELLYQUEEN_DASH_SPEED;

			m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
			m_direction.Normalize();

			m_followTarget = false;
			m_isDashing = true;
			m_dashTimer = 0.0f;
			StartSpecialAbilityCooldown();
		}
	}
}
