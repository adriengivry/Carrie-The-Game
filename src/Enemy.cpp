#include "Enemy.h"
#include "StateManager.h"

Enemy::Enemy(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Actor(p_sharedContext, p_x, p_y),
	m_target(nullptr),
	m_followTarget(true),
	m_damagesOnContact(true),
	m_isReady(true)
{
	SetTarget(m_sharedContext->m_actorManager->GetPlayer());
	++m_sharedContext->m_gameInfo->m_spawnedEnemies;
}

Enemy::~Enemy() {}

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
	Player* player = m_sharedContext->m_actorManager->GetPlayer();
	bool collide = false;

	// Hitting wall
	if (m_position.X() < 70)
	{
		m_position.X(70);
		collide = true;
	}

	if (m_position.X() > 1850)
	{
		m_position.X(1850);
		collide = true;
	}

	if (m_position.Y() < 300)
	{
		m_position.Y(300);
		collide = true;
	}

	if (m_position.Y() > 950)
	{
		m_position.Y(950);
		collide = true;
	}

	if (collide)
		return;

	// Out of screen
	if (m_position.X() < -100 || m_position.X() > 2000 || m_position.Y() < -100 || m_position.Y() > 1200)
	{
		m_mustDie = true;
		return;
	}

	if (m_target && m_followTarget)
	{
		m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
		m_direction.Normalize();
	}

	Actor::Update(l_time);	
}

void Enemy::Draw() const
{
	Actor::Draw();
	DrawLifebar();
}

void Enemy::DrawLifebar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getGlobalBounds().width, 5);

	rect.setPosition(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top - 25);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_life / m_maxLife;
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}