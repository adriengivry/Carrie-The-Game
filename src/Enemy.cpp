#include "Enemy.h"
#include "StateManager.h"

Enemy::Enemy(SharedContext* p_sharedContext, const float p_x, const float p_y) :
	Actor(p_sharedContext, p_x, p_y),
	m_target(nullptr),
	m_followTarget(true),
	m_damagesOnContact(true),
	m_showLifeBar(false),
	m_showSpecialAttackBar(false),
	m_showSpecialAbilityBar(false),
	m_isDead(false),
	m_deathTimer(0),
	m_deathDuration(0.5f),
	m_canCollide(true),
	m_isSpecialAttackReady(true),
	m_isSpecialAbilityReady(true)
{
	SetTarget(m_sharedContext->m_actorManager->GetPlayer());
	++m_sharedContext->m_gameInfo->m_spawnedEnemies;

	m_sprite.setScale(0.1f, 0.1f);

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

bool Enemy::IsDead() const
{
	return m_isDead;
}

void Enemy::RemoveLife(const float p_damages, const Vector2D<float> p_pushDirection)
{
	m_life -= p_damages;

	AddImpulsion(p_pushDirection);

	if (m_life <= 0 && !m_isDead)
	{
		m_life = 0;
		m_isDead = true;
		OnDeath();
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
	if (m_mustDie)
		return;

	if (m_sprite.getScale().x < 1)
		m_sprite.setScale(m_sprite.getScale().x + 5 * l_time.asSeconds(), m_sprite.getScale().y + 5 * l_time.asSeconds());
	else
		m_sprite.setScale(1.f, 1.f);

	m_velocityMultiplicator = 1;

	if (IsIntersecting(m_sharedContext->m_actorManager->GetPlayer()))
		m_velocityMultiplicator = 0;

	Actor::Update(l_time);

	if (!m_isDead)
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
	}
	else
	{
		if (!m_mustDie)
		{
			m_direction.Set(0, 0);
			m_deathTimer += l_time.asSeconds();
			if (m_deathTimer >= m_deathDuration)
				m_mustDie = true;
			
			sf::Color previousColor = m_sprite.getColor();
			previousColor.a = 255 - 255 * (m_deathTimer / m_deathDuration);
			m_sprite.setColor(previousColor);
		}
	}

	if (m_pushVelocity >= m_pushMaxVelocity / 2)
	{
		sf::Color previousColor = m_sprite.getColor();
		previousColor.r = 255;
		previousColor.g = 0;
		previousColor.b = 0;
		m_sprite.setColor(previousColor);
	}
	else
	{
		sf::Color previousColor = m_sprite.getColor();
		previousColor.r = 255;
		previousColor.g = 255;
		previousColor.b = 255;
		m_sprite.setColor(previousColor);
	}
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
	{
		const Vector2D<float> pushDirection(1, m_position.AngleTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()), POLAR);
		m_sharedContext->m_actorManager->GetPlayer()->RemoveLife(m_damages, false, pushDirection);
	}
}

float Enemy::CalculateStat(const float p_baseValue, const float p_valueIncrement, const float p_maxValue) const
{
	const float result = p_baseValue * pow(p_valueIncrement, m_sharedContext->m_gameInfo->m_currentLevel);
	return result > p_maxValue ? p_maxValue : result;
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

	if (m_showLifeBar || m_sharedContext->m_gameInfo->m_debugMode)
		DrawLifebar();

	if ((m_showSpecialAttackBar || m_sharedContext->m_gameInfo->m_debugMode) && m_specialAttackCooldown)
		DrawSpecialAttackCooldownBar();

	if ((m_showSpecialAbilityBar || m_sharedContext->m_gameInfo->m_debugMode) && m_specialAbilityCooldown)
		DrawSpecialAbilityCooldownBar();
}

void Enemy::DrawLifebar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getLocalBounds().width > __ENEMY_MIN_BAR_SIZE ? m_sprite.getLocalBounds().width : __ENEMY_MIN_BAR_SIZE, 10);

	rect.setPosition(m_position.X(), m_sprite.getGlobalBounds().top - 20);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	Utils::centerOrigin(rect);

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Red);
	barSize.x *= m_life / m_maxLife;
	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}

void Enemy::DrawSpecialAttackCooldownBar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getLocalBounds().width > __ENEMY_MIN_BAR_SIZE ? m_sprite.getLocalBounds().width : __ENEMY_MIN_BAR_SIZE, 10);

	rect.setPosition(m_position.X(), m_sprite.getGlobalBounds().top - 40);

	rect.setFillColor(sf::Color::White);
	rect.setSize(barSize);
	Utils::centerOrigin(rect);

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);

	rect.setFillColor(sf::Color::Green);

	if (!m_isSpecialAttackReady)
		barSize.x *= m_specialAttackTimer / m_specialAttackCooldown;

	rect.setSize(barSize);
	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}

void Enemy::DrawSpecialAbilityCooldownBar() const
{
	sf::RectangleShape rect;
	sf::Vector2f barSize = sf::Vector2f(m_sprite.getLocalBounds().width > __ENEMY_MIN_BAR_SIZE ? m_sprite.getLocalBounds().width : __ENEMY_MIN_BAR_SIZE, 10);

	rect.setPosition(m_position.X(), m_sprite.getGlobalBounds().top - 60);

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