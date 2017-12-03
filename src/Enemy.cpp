#include "Enemy.h"
#include "StateManager.h"

Enemy::Enemy(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Actor(p_sharedContext, p_x, p_y),
	m_target(nullptr),
	m_followTarget(true),
	m_damagesOnContact(true),
	m_isReady(true),
	m_canCollide(true)
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
	CheckCooldown(l_time);

	if (m_target && m_followTarget)
		FollowTarget();

	if (m_canCollide)
		CheckCollisions();

	if (m_isReady)
		Attack();

	Actor::Update(l_time);	
}

void Enemy::StartCooldown()
{
	m_timer = 0.f;
	m_isReady = false;
}

void Enemy::Attack()
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_damagesOnContact && this->IsIntersecting(player) && !player->IsInvulnerable())
	{
		player->RemoveLife(m_damages);
		StartCooldown();
	}
}

void Enemy::FollowTarget()
{
	m_direction.Set(1, m_position.AngleTo(m_target->GetPosition()), AGMath::POLAR);
	m_direction.Normalize();
}

void Enemy::CheckCollisions()
{
	// Hitting wall
	if (m_position.X() < 70)
		m_position.X(70);

	if (m_position.X() > 1850)
		m_position.X(1850);

	if (m_position.Y() < 300)
		m_position.Y(300);

	if (m_position.Y() > 950)
		m_position.Y(950);

	// Out of screen
	if (m_position.X() < -100 || m_position.X() > 2000 || m_position.Y() < -100 || m_position.Y() > 1200)
		m_mustDie = true;
}

void Enemy::CheckCooldown(const sf::Time& l_time)
{
	if (!m_isReady)
		m_timer += l_time.asSeconds();

	if (m_timer >= m_cooldown)
		m_isReady = true;
}

void Enemy::Draw() const
{
	Actor::Draw();
	DrawLifebar();
	DrawCooldownBar();
}

void Enemy::DrawLifebar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getGlobalBounds().width, 5);

	rect.setPosition(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top - 30);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_life / m_maxLife;
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}

void Enemy::DrawCooldownBar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getGlobalBounds().width, 5);

	rect.setPosition(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top - 25);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Blue);

	if (!m_isReady)
		barSize.x *= m_timer / m_cooldown;

	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}