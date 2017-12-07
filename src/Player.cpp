#include "Player.h"
#include "StateManager.h"

Player::Player(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Fire", &Player::Fire, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Unfire", &Player::Unfire, this);

	m_orientable = false;
	m_maxVelocity = __PLAYER_SPEED;

	m_acceleration = __PLAYER_ACCELERATION;

	m_maxLife = __PLAYER_LIFE;
	m_life = m_maxLife;

	m_invulnerable = false;
	m_invulnerableTimer = 0;

	m_reorienteTimer = 0;

	m_collide = false;

	m_isTransparent = false;
	m_invulnerableFlashTimer = 0.f;

	m_sprite.setScale(0.7f, 0.7f);
	m_shadowScale.Set(0.8f, 0.8f);

	m_fireOn = false;
	m_fireTimer = 0;
	m_fireCooldown = 0.2f;

	SetTexture(__PLAYER_TEXTURE);
}

Player::~Player()
{
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Fire");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Unfire");
}

void Player::Move(const sf::Time& l_time)
{
	if (!m_sharedContext->m_gameInfo->m_doorPassed)
	{
		Vector2D<float> direction;

		if (m_moveLeft)
			direction.X() -= 1;
		if (m_moveRight)
			direction.X() += 1;
		if (m_moveUp)
			direction.Y() -= 1;
		if (m_moveDown)
			direction.Y() += 1;

		if (abs(direction.X()) + abs(direction.Y()) == 2 && !m_collide)
		{
			direction.X() /= 2;
			direction.Y() /= 2;
		}

		if (m_moveLeft || m_moveRight || m_moveDown || m_moveUp)
		{
			m_direction = direction;
			m_acceleration = abs(m_acceleration);
		}
		else
			m_acceleration = abs(m_acceleration) * -1;

		if (m_reorienteTimer >= 0.25f)
		{
			if (direction.X() > 0 && direction.Y() == 0)		SetTexture("Carrie_Right");
			else if (direction.X() < 0 && direction.Y() == 0)	SetTexture("Carrie_Left");
			else if (direction.X() == 0 && direction.Y() > 0)	SetTexture("Carrie_Front");
			else if (direction.X() == 0 && direction.Y() < 0)	SetTexture("Carrie_Back");
			else if (direction.X() > 0 && direction.Y() > 0)	SetTexture("Carrie_Front_Right");
			else if (direction.X() < 0 && direction.Y() > 0)	SetTexture("Carrie_Front_Left");
			else if (direction.X() > 0 && direction.Y() < 0)	SetTexture("Carrie_Back_Right");
			else if (direction.X() < 0 && direction.Y() < 0)	SetTexture("Carrie_Back_Left");
		}

		Actor::Move(l_time);

		m_collide = false;

		bool xCollide = false;
		bool yCollide = false;

		if (m_position.X() < 120)
		{
			m_position.X(120);
			m_collide = true;
			xCollide = true;
		}

		if (m_position.X() > 1800)
		{
			m_position.X(1800);
			m_collide = true;
			xCollide = true;
		}

		if (m_position.Y() < 250)
		{
			m_position.Y(250);
			m_collide = true;
			yCollide = true;
		}

		if (m_position.Y() > 920)
		{
			m_position.Y(920);
			m_collide = true;
			yCollide = true;
		}

		if (!m_sharedContext->m_gameInfo->m_levelCompleted)
		{
			if (!xCollide)
				m_sharedContext->m_gameInfo->m_travelledDistance += abs(m_direction.X() * m_velocity * l_time.asSeconds()) / 100;

			if (!yCollide)
				m_sharedContext->m_gameInfo->m_travelledDistance += abs(m_direction.Y() * m_velocity * l_time.asSeconds()) / 100;
		}
	}
}

void Player::CheckFire()
{
	if (m_fireOn && m_fireTimer >= m_fireCooldown && !m_sharedContext->m_gameInfo->m_levelCompleted)
	{
		m_fireTimer = 0;
		GameInfo* gameInfo = m_sharedContext->m_gameInfo;
		Vector2D<float> mousePos;
		mousePos.Set(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

		Vector2D<float> projectileDirection;
		projectileDirection.Set(1, m_position.AngleTo(mousePos) + Utils::randomgen(0, 14 * gameInfo->m_reducedPrecision) - 7 * gameInfo->m_reducedPrecision, POLAR);

		std::cout << projectileDirection.GetAngle() << std::endl;

		if (projectileDirection.X() >= 0.f && projectileDirection.Y() <= 0.5f  && projectileDirection.Y() >= -0.5f)
		{
			SetTexture("Carrie_Right");
			m_reorienteTimer = 0;
		}
		else if (projectileDirection.X() < 0.f &&  projectileDirection.Y() <= 0.5f  && projectileDirection.Y() >= -0.5f)
		{
			SetTexture("Carrie_Left");
			m_reorienteTimer = 0;
		}
		else if (projectileDirection.Y() >= 0.f && projectileDirection.X() <= 0.5f  && projectileDirection.X() >= -0.5f)
		{
			SetTexture("Carrie_Front");
			m_reorienteTimer = 0;
		}
		else if (projectileDirection.Y() < 0.f && projectileDirection.X() <= 0.5f  && projectileDirection.X() >= -0.5f)
		{
			SetTexture("Carrie_Back");
			m_reorienteTimer = 0;
		}


		Projectile* newProjectile = new Projectile(m_sharedContext, projectileDirection, m_position.X(), m_position.Y() + 20);

		newProjectile->MultiplyDamages(pow(gameInfo->__PROJECTILE_WEAK_MULTIPLICATOR, gameInfo->m_weakerProjectiles));
		newProjectile->MultiplySpeed(pow(gameInfo->__PROJECTILE_SLOW_MULTIPLICATOR, gameInfo->m_slowerProjectiles));

		m_sharedContext->m_actorManager->AddProjectile(newProjectile);

		switch (Utils::randomgen(1, 3))
		{
		default:
		case 1:
			m_sharedContext->m_soundManager->PlaySound("Shoot_1");
			break;

		case 2:
			m_sharedContext->m_soundManager->PlaySound("Shoot_2");
			break;

		case 3:
			m_sharedContext->m_soundManager->PlaySound("Shoot_3");
			break;
		}
	}
}

void Player::Update(const sf::Time& l_time)
{
	Actor::Update(l_time);

	if (m_reorienteTimer < 10.f)
		m_reorienteTimer += l_time.asSeconds();

	m_fireTimer += l_time.asSeconds();
	
	CheckFire();
	CheckControls();

	m_velocityMultiplicator = pow(m_sharedContext->m_gameInfo->__CARRIE_SLOW_MULTIPLICATOR, m_sharedContext->m_gameInfo->m_slowerCarrie);

	if (IsInvulnerable())
	{
		m_invulnerableFlashTimer += l_time.asSeconds();
		if (m_invulnerableFlashTimer >= 0.1f)
		{
			m_isTransparent = !m_isTransparent;
			m_invulnerableFlashTimer = 0.0f;
		}

		m_invulnerableTimer += l_time.asSeconds();
		if (m_invulnerableTimer >= __INVULNERABLE_DURATION)
			m_invulnerable = false;
	}
	else
	{
		m_isTransparent = false;
	}

	if (m_isTransparent)
		m_sprite.setColor(sf::Color(255, 0, 0, 100));
	else
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
}

void Player::StopControl()
{
	m_moveDown = false;
	m_moveLeft = false;
	m_moveRight = false;
	m_moveUp = false;
}

void Player::Draw() const
{
	Actor::Draw();
}

float Player::GetMaxLife() const { return m_maxLife; }
float Player::GetLife() const { return m_life; }

void Player::MakeInvulnerable()
{
	m_invulnerable = true;
	m_invulnerableTimer = 0;
}

bool Player::IsInvulnerable() const { return m_invulnerable; }

void Player::CheckControls()
{
	const bool reverseMovement = m_sharedContext->m_gameInfo->m_reverseMovement;

	if (!reverseMovement)
	{
		m_moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		m_moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		m_moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		m_moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	}
	else
	{
		m_moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		m_moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		m_moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		m_moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	}
}

void Player::Fire(EventDetails* l_details)
{
	m_fireOn = true;
}

void Player::Unfire(EventDetails* l_details)
{
	m_fireOn = false;
}

void Player::RemoveLife(const float p_value, const bool p_constantDamages)
{
	if (!IsInvulnerable())
	{
		m_life -= p_value;
		if (!p_constantDamages)
			MakeInvulnerable();

		if (m_life <= 0)
		{
			m_life = 0;
			m_sharedContext->m_gameInfo->m_gameOver = true;
		}
	}
}
