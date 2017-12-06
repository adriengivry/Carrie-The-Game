#include "Actor.h"
#include "ActorManager.h"

Actor::Actor(SharedContext* p_sharedContext, const float p_x, const float p_y) : 
	m_textureGetSet(false),
	m_shadowOffset(__DEFAULT_SHADOW_OFFSET),
	m_sharedContext(p_sharedContext),
	m_position(p_x, p_y),
	m_direction(0, 0),
	m_velocity(__ACTOR_DEFAULT_VELOCITY),
	m_gotAShadow(true),
	m_shadowScale(1, 1),
	m_spriteScale(1, 1),
	m_flippable(false),
	m_orientable(false),
	m_mustDie(false) {}

Actor::~Actor() {}

void Actor::Update(const sf::Time& l_time)
{
	Move(l_time);
	if (m_textureGetSet)
		m_sprite.setPosition(m_position.ToSFVector());

	if (m_gotAShadow)
	{
		if (m_sharedContext->m_textureManager->RequireResource("Shadow"))
		{
			const float scaleFactor = m_sprite.getGlobalBounds().width / 75.f;
			m_shadow.setTexture(*m_sharedContext->m_textureManager->GetResource("Shadow"));
			m_shadow.setScale(scaleFactor, scaleFactor);
			m_shadow.scale(m_shadowScale.X(), m_shadowScale.Y());
			m_shadow.setPosition(m_position.X(), m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height - m_shadowOffset);
			m_shadow.setColor(sf::Color(255, 255, 255, 100));
			Utils::centerOrigin(m_shadow);
		}
	}
}

void Actor::Move(const sf::Time& l_time)
{
	Vector2D<float> previousPos = m_position;
	const Vector2D<float> newPos = m_position + m_direction * m_velocity * l_time.asSeconds();

	m_position = newPos;

	if (m_orientable)
		if (m_position.X() != previousPos.X() || m_position.Y() != previousPos.Y())
			if (m_textureGetSet)
				m_sprite.setRotation(m_direction.GetAngle());
}

void Actor::Draw() const
{
	if (m_textureGetSet)
	{
		if (m_gotAShadow)
			m_sharedContext->m_wind->GetRenderWindow()->draw(m_shadow);

		sf::Sprite toDraw = m_sprite;
		toDraw.scale(m_spriteScale.X(), m_spriteScale.Y());

		if (m_flippable && m_direction.X() < 0)
			toDraw.scale(-1, 1);

		m_sharedContext->m_wind->GetRenderWindow()->draw(toDraw);
	}
}

bool Actor::IsIntersecting(Actor* p_otherActor) const
{
	return m_position.DistanceTo(p_otherActor->GetPosition()) <= m_sprite.getGlobalBounds().width / 2 + p_otherActor->GetSprite().getGlobalBounds().width / 2 - (m_sprite.getGlobalBounds().width / 2 + p_otherActor->GetSprite().getGlobalBounds().width / 2) * 0.2f;
}

bool Actor::MustDie() const
{
	return m_mustDie;
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