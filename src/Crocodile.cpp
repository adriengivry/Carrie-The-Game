#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__CROCODILE_TEXTURE_GREEN);
	m_spriteScale.Set(1.5f, 1.5f);
	m_shadowScale.Set(1.f, 0.5f);

	m_shadowOffset = 0;

	m_flippable = true;

	Crocodile::GenerateStats();
	ResetLife();

	m_isDashing = false;
	m_dashTimer = 0.0f;
	m_dashMaxDuration = 0.4f;
}

Crocodile::~Crocodile()
{
	m_sharedContext->m_soundManager->PlaySound("Death_Crocodile");
}

void Crocodile::GenerateStats()
{
	m_velocity = __CROCODILE_SPEED;
	m_maxVelocity = m_velocity;

	m_specialAbilityCooldown = __CROCODILE_SPECIAL_ABILITY_COOLDOWN;

	GENERATE_LIFE(__CROCODILE_LIFE, __CROCODILE_LIFE_INCREMENTATION_COEFFICIENT, __CROCODILE_MAX_LIFE);
	GENERATE_DAMAGES(__CROCODILE_DAMAGES, __CROCODILE_DAMAGES_INCREMENTATION_COEFFICIENT, __CROCODILE_MAX_DAMAGES);
}

void Crocodile::Update(const sf::Time& l_time)
{
	if (m_isDashing)
	{
		SetTexture(__CROCODILE_TEXTURE_RED);
		m_specialAbilityTimer = 0.0f;
		m_velocity = __CROCODILE_DASH_SPEED;
		m_dashTimer += l_time.asSeconds();
		if (m_dashTimer >= m_dashMaxDuration)
			m_isDashing = false;
	}
	else
	{
		m_velocity = __CROCODILE_SPEED;
		m_followTarget = true;

		if (m_isSpecialAbilityReady)
			SetTexture(__CROCODILE_TEXTURE_RED);
		else
			SetTexture(__CROCODILE_TEXTURE_GREEN);
	}

	Enemy::Update(l_time);
}

void Crocodile::SpecialAbility(const sf::Time& l_time)
{
	if (m_position.DistanceTo(m_target->GetPosition()) < __CROCODILE_DASH_MIN_DISTANCE)
	{
		m_velocity = __CROCODILE_DASH_SPEED;

		m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
		m_direction.Normalize();

		m_followTarget = false;
		m_isDashing = true;
		m_dashTimer = 0.0f;
		StartSpecialAbilityCooldown();
	}	
}
