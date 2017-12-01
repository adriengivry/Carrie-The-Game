#include "Enemy.h"

Enemy::Enemy(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Actor(p_sharedContext, p_x, p_y),
	m_target(nullptr)
{
	SetTexture(__ENEMY_TEXTURE);
	m_velocity = __ENEMY_SPEED;

	m_direction.Set(1, Utils::randomgen(45, 135), POLAR);
	m_position.Set(Utils::randomgen(100, 1820), -50);
}

Enemy::~Enemy()
{
}

void Enemy::SetTarget(Actor* p_target)
{
	m_target = p_target;
}

void Enemy::Update(const sf::Time& l_time)
{
	if (m_target)
	{
		m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
		m_direction.Normalize();

		m_position += m_direction * m_velocity * l_time.asSeconds();

		m_sprite.setPosition(m_position.ToSFVector());
	}

	m_sprite.rotate(100 * l_time.asSeconds());

	Actor::Update(l_time);

	if (m_position.Y() >= 1200)
		m_mustDie = true;
}
