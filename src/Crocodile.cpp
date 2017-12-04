#include "Crocodile.h"
#include "StateManager.h"

Crocodile::Crocodile(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y),
	m_isJumping(false)
{
	SetTexture(__CROCODILE_TEXTURE);
	m_sprite.setScale(1.5f, 1.5f);

	m_flippable = true;

	const float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_shadowOffset = 75;
	m_shadowScale.Set(1.f, 0.5f);

	m_velocity = __CROCODILE_SPEED;
	m_cooldown = __CROCODILE_COOLDOWN;

	if (m_maxLife > 350)
		m_maxLife = 350;
	else
		m_maxLife = __CROCODILE_LIFE * level * 1.1f;

	if (m_damages > 33)
		m_damages = 33;
	else
		m_damages = __CROCODILE_DAMAGES * level * 1.05;

	m_life = m_maxLife;
	m_timer = 0;
}

Crocodile::~Crocodile() {}

void Crocodile::Update(const sf::Time & l_time)
{
	SetTexture(__CROCODILE_TEXTURE);

	if (m_isJumping)
	{
		m_jumpTimer += l_time.asSeconds();

		if (m_jumpTimer >= 0.6f)
		{
			m_isJumping = false;
			m_jumpTimer = 0.0f;
			m_velocity = __CROCODILE_SPEED;
			m_direction.Set(0, 0);
			m_followTarget = true;
			StartCooldown();
		}
	}

	if (m_isReady && !m_isJumping && inRange())
		Jump();

	if (m_isReady)
		SetTexture("CrocodileFrontRed");

	Enemy::Update(l_time);
}

bool Crocodile::inRange() const
{
	const float range = m_position.DistanceTo(m_target->GetPosition());

	if (range < 350)
		return true;

	return false;
}

void Crocodile::Jump()
{
	m_velocity *= 8;

	m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
	m_direction.Normalize();

	m_followTarget = false;
	m_isJumping = true;
}
