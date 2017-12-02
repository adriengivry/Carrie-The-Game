#include "Enemy.h"
#include "StateManager.h"

Enemy::Enemy(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Actor(p_sharedContext, p_x, p_y),
	m_target(nullptr)
{
	m_direction.Set(1, Utils::randomgen(45, 135), POLAR);

	SetTexture("Enemy");

	SetTarget(m_sharedContext->m_actorManager->GetPlayer());
}

Enemy::~Enemy()
{
}

void Enemy::SetTarget(Actor* p_target)
{
	m_target = p_target;
}

void Enemy::Kill()
{
	m_mustDie = true;
}

void Enemy::RemoveLife(const float p_damages)
{
	m_life -= p_damages;

	if (m_life <= 0)
	{
		m_life = 0;
		m_mustDie = true;
	}
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
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (this->IsIntersecting(player) && !player->IsInvulnerable())
	{
		player->RemoveLife(m_damages);
		player->MakeInvulnerable();
	}

	if (m_position.X() < 270 || m_position.X() > 1650 || m_position.Y() < 120 || m_position.Y() > 900)
		m_mustDie = true;
}

void Enemy::Draw() const
{
	Actor::Draw();

	DrawLifebar();
}

void Enemy::DrawLifebar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getGlobalBounds().width, 25);

	rect.setPosition(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top - 25);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_life / m_maxLife;
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}
