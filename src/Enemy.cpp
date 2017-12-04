#include "Enemy.h"
#include "StateManager.h"

Enemy::Enemy(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Actor(p_sharedContext, p_x, p_y),
	m_target(nullptr),
	m_followTarget(true),
	m_damagesOnContact(true),
	m_isSpecialAttackReady(true),
	m_isSpecialAbilityReady(true),
	m_canCollide(true),
	m_showCooldownBar(false)
{
	SetTarget(m_sharedContext->m_actorManager->GetPlayer());
	++m_sharedContext->m_gameInfo->m_spawnedEnemies;

	m_specialAttackCooldown = __ENEMY_DEFAULT_COOLDOWN;
	m_specialAbilityCooldown = __ENEMY_DEFAULT_COOLDOWN;
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

void Enemy::SpecialAttack(const sf::Time& l_time)
{
	// NO SPECIAL ATTACK (OVERRIDE TO APPLY NEW BEHAVIOR)
}

void Enemy::SpecialAbility(const sf::Time& l_time)
{
	// NO SPECIAL ABILITY (OVERRIDE TO APPLY NEW BEHAVIOR)
}

void Enemy::ResetLife()
{
	m_life = m_maxLife;
}

void Enemy::Update(const sf::Time& l_time)
{
	CheckSepcialAttackCooldown(l_time);
	CheckSpecialAbilityCooldown(l_time);

	if (m_target && m_followTarget)
		FollowTarget();

	if (m_canCollide)
		CheckCollisions();

	if (m_isSpecialAttackReady || m_specialAttackCooldown == 0)
	{
		SpecialAttack(l_time);
	}

	if (m_isSpecialAbilityReady || m_specialAbilityCooldown == 0)
	{
		SpecialAbility(l_time);
	}

	Attack();

	Actor::Update(l_time);	
}

void Enemy::StartSpecialAttackCooldown()
{
	m_specialAttackTimer = 0.f;
	m_isSpecialAttackReady = false;
}

void Enemy::StartSpecialAbilityCooldown()
{
	m_specialAbilityTimer = 0.f;
	m_isSpecialAbilityReady = false;
}

void Enemy::Attack()
{
	if (IsIntersecting(m_sharedContext->m_actorManager->GetPlayer()) && m_damagesOnContact)
		m_sharedContext->m_actorManager->GetPlayer()->RemoveLife(m_damages, false);
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

void Enemy::CheckSepcialAttackCooldown(const sf::Time& l_time)
{
	if (!m_isSpecialAttackReady)
		m_specialAttackTimer += l_time.asSeconds();

	if (m_specialAttackTimer >= m_specialAttackCooldown)
		m_isSpecialAttackReady = true;
}

void Enemy::CheckSpecialAbilityCooldown(const sf::Time& l_time)
{
	if (!m_isSpecialAbilityReady)
		m_specialAbilityTimer += l_time.asSeconds();

	if (m_specialAbilityTimer >= m_specialAbilityCooldown)
		m_isSpecialAbilityReady = true;
}

void Enemy::Draw() const
{
	Actor::Draw();
	DrawLifebar();
	if (m_showCooldownBar && m_specialAbilityCooldown != 0.f)
		DrawCooldownBar();
}

void Enemy::DrawLifebar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(100, 10);

	rect.setPosition(m_position.X(), m_sprite.getGlobalBounds().top - 35);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	Utils::centerOrigin(rect);

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_life / m_maxLife;
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}

void Enemy::DrawCooldownBar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(100, 10);

	rect.setPosition(m_position.X(), m_sprite.getGlobalBounds().top - 20);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	Utils::centerOrigin(rect);

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Blue);

	if (!m_isSpecialAbilityReady)
		barSize.x *= m_specialAbilityTimer / m_specialAbilityCooldown;

	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}