#include "Actor.h"
#include "ActorManager.h"

Actor::Actor(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	m_textureGetSet(false),
	m_sharedContext(p_sharedContext),
	m_position(p_x, p_y),
	m_direction(0, 0), 
	m_zBuffer(0),
	m_velocity(0),
	m_maxVelocity(__ACTOR_DEFAULT_VELOCITY),
	m_acceleration(0),
	m_velocityMultiplicator(1),
	m_spriteScale(1, 1),
	m_flippable(false),
	m_orientable(false),
	m_mustDie(false),
	m_lifetimeCounter(0)
{
	m_pushAcceleration = -1000;
	m_pushMaxVelocity = 300;
	m_pushVelocity = 0;
	m_pushDirection.Set(0, 0);

	m_particleSpawnTimer = 0;
	m_particleSpawnDelay = 0.03f;
}

Actor::~Actor() {}

void Actor::Update(const sf::Time& l_time)
{
	m_lifetimeCounter += l_time.asSeconds();

	m_particleSpawnTimer += l_time.asSeconds();
	if (m_particleSpawnTimer >= m_particleSpawnDelay && m_velocity >= 50)
	{
		SpawnParticle();
	}

	m_zBuffer = m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height;

	if (m_acceleration != 0)
	{
		m_velocity += m_acceleration * l_time.asSeconds();

		if (m_velocity > m_maxVelocity)
			m_velocity = m_maxVelocity;

		if (m_velocity <= 0)
		{
			m_velocity = 0;
			m_acceleration *= -1;
		}
	}

	Move(l_time);
	if (m_textureGetSet)
		m_sprite.setPosition(m_position.ToSFVector());
}

void Actor::Move(const sf::Time& l_time)
{
	Vector2D<float> previousPos = m_position;
	const Vector2D<float> newPos = m_position + m_direction * m_velocity * m_velocityMultiplicator * l_time.asSeconds();

	m_position = newPos;

	if (m_orientable)
		if (m_position.X() != previousPos.X() || m_position.Y() != previousPos.Y())
			if (m_textureGetSet)
				m_sprite.setRotation(m_direction.GetAngle());

	if (m_pushVelocity > 0)
		m_pushVelocity += m_pushAcceleration * l_time.asSeconds();
	else
		m_pushVelocity = 0;

	m_position += m_pushDirection * m_pushVelocity * l_time.asSeconds();
}

void Actor::Draw() const
{
	if (m_textureGetSet)
	{
		sf::Sprite toDraw = m_sprite;

		if (m_flippable && m_direction.X() < 0)
			toDraw.scale(-1, 1);

		m_sharedContext->m_wind->GetRenderWindow()->draw(toDraw);
	}
}

void Actor::SpawnParticle()
{
	// TO OVERRIDE
}

bool Actor::IsIntersecting(Actor* p_otherActor) const
{
	sf::Rect<float> actorCollider = m_sprite.getGlobalBounds();

	actorCollider.width *= 0.75;
	actorCollider.height *= 0.75;

	actorCollider.left += actorCollider.width * 0.25 / 2;
	actorCollider.top += actorCollider.height * 0.25 / 2;

	sf::Rect<float> otherActorCollider = p_otherActor->GetSprite().getGlobalBounds();

	otherActorCollider.width *= 0.75;
	otherActorCollider.height *= 0.75;

	otherActorCollider.left += otherActorCollider.width * 0.25 / 2;
	otherActorCollider.top += otherActorCollider.height * 0.25 / 2;

	return actorCollider.intersects(otherActorCollider);
}

bool Actor::MustDie() const
{
	return m_mustDie;
}

void Actor::AddImpulsion(const Vector2D<float> p_direction, const float p_power)
{
	m_pushMaxVelocity = p_power;
	m_pushVelocity = m_pushMaxVelocity;
	m_pushDirection = p_direction;
}

void Actor::SetVelocity(const float p_newVelocity)
{
	m_velocity = p_newVelocity;
}

void Actor::SetDirection(const float p_x, const float p_y)
{
	m_direction.Set(p_x, p_y);
	m_direction.Normalize();
}

void Actor::SetDirection(const float p_angle)
{
	m_direction.Set(1, p_angle, POLAR);
	m_direction.Normalize();
}

void Actor::SetPosition(const float p_x, const float p_y)
{
	m_position.Set(p_x, p_y);
}

void Actor::SetTexture(const std::string p_textureName)
{
	if (m_sharedContext->m_textureManager->RequireResource(p_textureName))
	{
		m_sprite.setTexture(*m_sharedContext->m_textureManager->GetResource(p_textureName));
		Utils::centerOrigin(m_sprite);
		m_textureGetSet = true;
	}
}

float Actor::GetVelocity() const { return m_velocity; }
sf::Sprite& Actor::GetSprite() { return m_sprite; }
uint16_t& Actor::GetZBuffer() { return m_zBuffer; }
Vector2D<float>& Actor::GetPosition() { return m_position; }
Vector2D<float>& Actor::GetDirection() { return m_direction; }