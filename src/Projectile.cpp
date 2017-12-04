#include "Projectile.h"
#include "StateManager.h"

Projectile::Projectile(SharedContext* p_sharedContext, const Vector2D<float> p_direction, const float p_x, const float p_y, const bool p_friendly, const bool p_isLaser)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_direction = p_direction;
	m_velocity = __PROJECTILE_SPEED;
	m_damages = __PROJECTILE_DAMAGES;
	m_hitrate = __PROJECTILE_HITRATE;

	m_constantDamages = p_isLaser;

	m_friendly = p_friendly;

	if (m_friendly)
	{
		SetTexture(__PROJECTILE_TEXTURE);
		++m_sharedContext->m_gameInfo->m_spawnedProjectiles;
		m_sprite.scale(0.7f, 0.7f);
	}
	else
	{
		if (p_isLaser)
		{
			SetTexture(__ENEMY_PROJECTILE_LASER_TEXTURE);
			m_gotAShadow = false;
			m_sprite.scale(0.7f, 0.7f);
		}
		else
		{
			SetTexture(__ENEMY_PROJECTILE_TEXTURE);
			m_sprite.scale(1.2f, 1.2f);
		}
	}

	

	m_orientable = false;
}

Projectile::~Projectile()
{
}

void Projectile::Update(const sf::Time& l_time)
{
	Actor::Update(l_time);

	m_sprite.rotate(350 * l_time.asSeconds());

	if (m_friendly)
	{
		for (auto enemy : m_sharedContext->m_actorManager->GetEnemies())
		{
			if (IsIntersecting(enemy) && !MustDie())
			{
				m_mustDie = true;
				enemy->RemoveLife(m_damages);
			}
		}
	}
	else
	{
		if (IsIntersecting(m_sharedContext->m_actorManager->GetPlayer()) && !MustDie())
		{
			m_mustDie = true;
			m_sharedContext->m_actorManager->GetPlayer()->RemoveLife(m_damages, m_constantDamages);
		}
	}

	if (m_position.X() < 50 || m_position.X() > 1870 || m_position.Y() < 200 || m_position.Y() > 1000)
		m_mustDie = true;
}

void Projectile::MultiplyDamages(const float p_value)
{
	m_damages *= p_value;
}

void Projectile::MultiplySpeed(const float p_value)
{
	m_velocity *= p_value;
}

void Projectile::MultiplyHitrate(const float p_value)
{
	m_hitrate *= p_value;
}

bool Projectile::IsFriendly() const
{
	return m_friendly;
}

void Projectile::SetDamages(const float p_value)
{
	m_damages = p_value;
}

void Projectile::SetSpeed(const float p_speed)
{
	m_velocity = p_speed;
}
