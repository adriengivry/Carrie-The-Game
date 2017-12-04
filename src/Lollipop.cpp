#include "Lollipop.h"
#include "StateManager.h"

Lollipop::Lollipop(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__LOLLIPOP_TEXTURE);

	Lollipop::GenerateStats();
	ResetLife();
	
	m_followTarget = false;
	m_isDashing = false;
	m_dashTimer = 0.0f;
	m_dashMaxDuration = 0.2f;
	m_showCooldownBar = true;
}

Lollipop::~Lollipop() {}

void Lollipop::GenerateStats()
{
	m_velocity = __LOLLIPOP_SPEED;
	m_specialAttackCooldown = __LOLLIPOP_SPECIAL_ATTACK_COOLDOWN;
	m_specialAbilityCooldown = __LOLLIPOP_SPECIAL_ABILITY_COOLDOWN;

	GENERATE_LIFE(__LOLLIPOP_LIFE, __LOLLIPOP_LIFE_INCREMENTATION_COEFFICIENT, __LOLLIPOP_MAX_LIFE);
	GENERATE_DAMAGES(__LOLLIPOP_DAMAGES, __LOLLIPOP_DAMAGES_INCREMENTATION_COEFFICIENT, __LOLLIPOP_MAX_DAMAGES);
}

void Lollipop::Update(const sf::Time& l_time)
{
	if (m_isDashing)
	{
		m_specialAbilityTimer = 0.0f;
		m_velocity = __LOLLIPOP_DASH_SPEED;
		m_dashTimer += l_time.asSeconds();
		if (m_dashTimer >= m_dashMaxDuration)
			m_isDashing = false;
	}
	else
	{
		m_direction.Set(0, 0);
		m_velocity = __LOLLIPOP_SPEED;
	}

	Enemy::Update(l_time);
}

void Lollipop::SpecialAttack(const sf::Time& l_time)
{
	Vector2D<float> projectileDirection;
	projectileDirection.Set(1, m_position.AngleTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()), POLAR);

	Projectile* projectile = new Projectile(m_sharedContext, projectileDirection, m_position.X(), m_position.Y(), false, false);
	projectile->SetDamages(m_damages);
	projectile->SetSpeed(__LOLLIPOP_PROJECTILE_SPEED);

	m_sharedContext->m_actorManager->AddProjectile(projectile);

	StartSpecialAttackCooldown();
}

void Lollipop::SpecialAbility(const sf::Time& l_time)
{
	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (it->IsFriendly() &&
			!it->MustDie() &&
			m_position.DistanceTo(it->GetPosition()) <= __LOLLIPOP_DASH_MIN_DISTANCE)
		{
			m_velocity = __LOLLIPOP_DASH_SPEED;
			const uint8_t rand = Utils::randomgen(0, 1);

			if (m_position.AngleTo(it->GetPosition()) < 5)
			{
				m_direction.X() = -it->GetDirection().Y();
				m_direction.Y() = it->GetDirection().X();
			}
			else if (m_position.AngleTo(it->GetPosition()) > -5)
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