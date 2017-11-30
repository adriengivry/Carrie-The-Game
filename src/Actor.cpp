#include "Actor.h"


Actor::Actor(SharedContext* p_sharedContext, const float p_x, const float p_y) : 
	m_textureGetSet(false),
	m_sharedContext(p_sharedContext),
	m_position(p_x, p_y),
	m_direction(0, 0),
	m_velocity(__ACTOR_DEFAULT_VELOCITY) {}

Actor::~Actor() {}

void Actor::Update(const sf::Time& l_time)
{
	Move(l_time);
	if (m_textureGetSet)
		m_sprite.setPosition(m_position.ToSFVector());
}

void Actor::Move(const sf::Time& l_time)
{
	Vector2D<float> m_previousPos = m_position;
	m_position += m_direction * m_velocity * l_time.asSeconds();

	if (m_position.X() != m_previousPos.X() || m_position.Y() != m_previousPos.Y())
	{
		if (m_textureGetSet)
			m_sprite.setRotation(m_direction.GetAngle());
	}
}

void Actor::Draw() const
{
	if (m_textureGetSet)
		m_sharedContext->m_wind->GetRenderWindow()->draw(m_sprite);
}

bool Actor::IsIntersecting(Actor* p_otherActor) const
{
	return m_position.DistanceTo(p_otherActor->GetPosition()) <= 120;
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

sf::Sprite& Actor::GetSprite() { return m_sprite; }
Vector2D<float>& Actor::GetPosition() { return m_position; }
Vector2D<float>& Actor::GetDirection() { return m_direction; }