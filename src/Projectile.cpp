#include "Projectile.h"
#include "StateManager.h"


Projectile::Projectile(SharedContext* p_sharedContext, const Vector2D<float> p_direction, 
	Actor* p_source,
	const float p_x, const float p_y, 
	const ProjectileType p_projectileType,
	const std::string p_texture, 
	const float p_scaleX, const float p_scaleY)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_direction = p_direction;
	m_velocity = __PROJECTILE_SPEED;
	m_maxVelocity = m_velocity;
	m_damages = __PROJECTILE_DAMAGES;

	SetTexture(p_texture);

	m_projectileType = p_projectileType;

	m_source = p_source;

	m_constantlyRotate = false;
}

Projectile::~Projectile()
{
}

void Projectile::Update(const sf::Time& l_time)
{
	Actor::Update(l_time);

	m_zBuffer += 20;

	if (m_constantlyRotate)
		m_sprite.rotate(350 * l_time.asSeconds());

	if (!MustDie())
	{
		if (IsFriendly())
		{
			for (auto enemy : m_sharedContext->m_actorManager->GetEnemies())
			{
				if (!enemy->IsDead() && IsIntersecting(enemy) && !MustDie())
				{
					m_mustDie = true;
					enemy->RemoveLife(m_damages, m_direction);
				}
			}

			for (auto projectile : m_sharedContext->m_actorManager->GetProjectile())
			{
				if (!projectile->IsFriendly() && IsIntersecting(projectile))
				{
					if (projectile->GetType() == ProjectileType::SUGAR)
					{
						projectile->Kill();
						if (m_sprite.getScale().x <= 3.f)
						{
							m_sprite.scale(1.3f, 1.3f);
							m_damages *= 1.3f;
						}
					}
					else if (projectile->GetType() == ProjectileType::CREAM || projectile->GetType() == ProjectileType::LASER_CREAM)
					{
						if (m_sprite.getScale().x >= 0.5f)
						{
							m_velocityMultiplicator *= 1 - 0.9 * l_time.asSeconds();
							m_damages *= 1 - 0.9 * l_time.asSeconds();
							m_sprite.scale(1 - 0.9 * l_time.asSeconds(), 1 - 0.9 * l_time.asSeconds());
						}
					}
					else if (projectile->GetType() == ProjectileType::NORMAL)
					{
						projectile->Kill();
					}
					else if (projectile->GetType() == ProjectileType::STRONG)
					{
						m_mustDie = true;
					}
				}
			}
		}
		else
		{
			for (auto enemy : m_sharedContext->m_actorManager->GetEnemies())
			{
				if (!enemy->IsDead() && IsIntersecting(enemy) && !MustDie() && enemy != m_source)
				{
					m_mustDie = true;
				}
			}

			if (IsIntersecting(m_sharedContext->m_actorManager->GetPlayer()))
			{
				m_mustDie = true;
				m_sharedContext->m_actorManager->GetPlayer()->RemoveLife(m_damages, DealsConstantDamages(), m_direction);
			}
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

void Projectile::Kill()
{
	m_mustDie = true;
}

void Projectile::SpawnParticle()
{
	if (m_projectileType != ProjectileType::LASER && m_projectileType != ProjectileType::LASER_CREAM)
	{
		for (uint8_t i = 0; i < 8; ++i)
		{
			const float particleSize = Utils::randomgen(3, 5);
			const float xOffset = Utils::randomgen(0, 20) - 10;
			const float yOffset = Utils::randomgen(0, 20) - 10;
			const float angle = Utils::randomgen(0, 360);

			uint8_t r;
			uint8_t g;
			uint8_t b;

			if (IsFriendly() && Utils::randomgen(0, 1) == 0)
			{
				r = 83;
				g = 47;
				b = 30;
			}
			else
			{
				r = 255;
				g = 255;
				b = 255;
			}

			const uint8_t a = Utils::randomgen(50, 150);

			m_sharedContext->m_actorManager->AddParticle(new Particle(m_sharedContext, m_position.X() + xOffset, m_position.Y() + m_shadowOffset + yOffset, particleSize, particleSize, angle, sf::Color(r, g, b, a), 0.4f), ParticleType::PROJECTILE);
			m_particleSpawnTimer = 0;
		}
	}
}

Actor* Projectile::GetSource() const
{
	return m_source;
}

ProjectileType Projectile::GetType() const
{
	return m_projectileType;
}

bool Projectile::IsFriendly() const
{
	return m_source == m_sharedContext->m_actorManager->GetPlayer();
}

bool Projectile::DealsConstantDamages() const
{
	return m_projectileType == ProjectileType::LASER || m_projectileType == ProjectileType::LASER_CREAM;
}

void Projectile::SetDamages(const float p_value)
{
	m_damages = p_value;
}

void Projectile::SetSpeed(const float p_speed)
{
	m_velocity = p_speed;
}

void Projectile::SetConstantlyRotate(const bool p_state)
{
	m_constantlyRotate = p_state;
}

void Projectile::SetColor(const sf::Color p_color)
{
	m_sprite.setColor(p_color);
}
