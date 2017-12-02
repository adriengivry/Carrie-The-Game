#include "Projectile.h"
#include "StateManager.h"

Projectile::Projectile(SharedContext* p_sharedContext, const Vector2D<float> p_direction, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_direction = p_direction;
	m_velocity = __PROJECTILE_SPEED;
	m_damages = __PROJECTILE_DAMAGES;
	m_hitrate = __PROJECTILE_HITRATE;

	SetTexture(__PROJECTILE_TEXTURE);

	m_orientable = true;
}

Projectile::~Projectile()
{
}

void Projectile::Update(const sf::Time& l_time)
{
	Actor::Update(l_time);

	for (auto enemy : m_sharedContext->m_actorManager->GetEnemies())
	{
		if (IsIntersecting(enemy))
		{
			m_mustDie = true;
			enemy->RemoveLife(m_damages);
		}
	}

	if (m_position.X() < 270 || m_position.X() > 1650 || m_position.Y() < 120 || m_position.Y() > 900)
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
