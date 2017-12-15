#include "Player.h"
#include "StateManager.h"

Player::Player(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y),
	m_maxLife(m_sharedContext->m_gameInfo->m_playerMaxLife),
	m_life(m_sharedContext->m_gameInfo->m_playerLife)
{
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Fire", &Player::Fire, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Unfire", &Player::Unfire, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Use_Special_Ability", &Player::UseSpecialAbility, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Release_Special_Ability", &Player::ReleaseSpecialAbility, this);

	m_orientable = false;
	m_maxVelocity = __PLAYER_SPEED;

	m_acceleration = __PLAYER_ACCELERATION;

	m_invulnerable = false;
	m_getHit = false;
	
	m_timeSinceLastHit = 0;

	m_reorienteTimer = 0;

	m_collide = false;

	m_isTransparent = false;
	m_getHitFlashTimer = 0.f;

	m_fireOn = false;
	m_fireTimer = 0;
	m_fireCooldown = 0.2f;

	m_xScaleIncrement = 1.5f;
	m_yScaleIncrement = 1.5f;

	SetTexture(__PLAYER_TEXTURE);
}

Player::~Player()
{
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Fire");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Unfire");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Use_Special_Ability");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Release_Special_Ability");
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
			else if (direction.X() > 0 && direction.Y() > 0)	SetTexture("Carrie_Front");
			else if (direction.X() < 0 && direction.Y() > 0)	SetTexture("Carrie_Front");
			else if (direction.X() > 0 && direction.Y() < 0)	SetTexture("Carrie_Back");
			else if (direction.X() < 0 && direction.Y() < 0)	SetTexture("Carrie_Back");
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

		std::string projectileTexture;
		ProjectileType projectileType;

		if (gameInfo->m_perforantChocolateUpgrade)
		{
			projectileTexture = "Perforant_Chocolate";
			projectileType = ProjectileType::PERFORANT;
		}
		else
		{
			projectileTexture = "Chocolate";
			projectileType = ProjectileType::NORMAL;
		}

		float angleOffset[]{ 0, -5, 5 };

		uint8_t projectileToThrow = 1;
		if (gameInfo->m_tripleChocolateUpgrade)
			projectileToThrow = 3;

		for (uint8_t i = 0; i < projectileToThrow; ++i)
		{
			projectileDirection.Set(1, m_position.AngleTo(mousePos) + angleOffset[i] + Utils::randomgen(0, 14 * gameInfo->m_reducedPrecision) - 7 * gameInfo->m_reducedPrecision, POLAR);
			Projectile* newProjectile = new Projectile(m_sharedContext, projectileDirection, this, m_position.X(), m_position.Y() + 20, projectileType, projectileTexture);

			if (gameInfo->m_perforantChocolateUpgrade)
			{
				newProjectile->GetSprite().rotate(projectileDirection.GetAngle());
			}
			else
			{
				newProjectile->SetConstantlyRotate(true);
			}

			newProjectile->MultiplyDamages(pow(gameInfo->__PROJECTILE_WEAK_MULTIPLICATOR, gameInfo->m_weakerProjectiles));
			newProjectile->MultiplySpeed(pow(gameInfo->__PROJECTILE_SLOW_MULTIPLICATOR, gameInfo->m_slowerProjectiles));
			++gameInfo->m_spawnedProjectiles;

			m_sharedContext->m_actorManager->AddProjectile(newProjectile);
		}

		switch (Utils::randomgen(1, 3))
		{
		default:
		case 1:
			m_sharedContext->m_audioManager->PlaySound("Shoot_1");
			break;

		case 2:
			m_sharedContext->m_audioManager->PlaySound("Shoot_2");
			break;

		case 3:
			m_sharedContext->m_audioManager->PlaySound("Shoot_3");
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

	if (m_getHit)
	{
		m_getHitFlashTimer += l_time.asSeconds();
		if (m_getHitFlashTimer >= 0.1f)
		{
			m_isTransparent = !m_isTransparent;
			m_getHitFlashTimer = 0.0f;
		}

		m_timeSinceLastHit += l_time.asSeconds();
		if (m_timeSinceLastHit >= __GETHIT_DURATION)
		{
			m_invulnerable = false;
			m_getHit = false;
		}
	}
	else
	{
		m_isTransparent = false;
	}

	if (m_isTransparent)
		if (m_invulnerable)
			m_sprite.setColor(sf::Color(255, 0, 0, 100));
		else
			m_sprite.setColor(sf::Color(0, 0, 255, 100));
	else
		m_sprite.setColor(sf::Color(255, 255, 255, 255));

	if (m_moveLeft || m_moveRight || m_moveUp || m_moveDown)
	{
		if (m_moveUp || m_moveDown)
		{
			m_spriteScale.Y() += m_yScaleIncrement * l_time.asSeconds();

			if (m_spriteScale.Y() >= 1.1f && m_yScaleIncrement > 0)
				m_yScaleIncrement = -abs(m_yScaleIncrement);
			else if (m_spriteScale.Y() <= 0.9f && m_yScaleIncrement < 0)
				m_yScaleIncrement = abs(m_yScaleIncrement);
		}
		else if (m_moveLeft || m_moveRight)
		{
			m_spriteScale.X() += m_xScaleIncrement * l_time.asSeconds();

			if (m_spriteScale.X() >= 1.1f && m_xScaleIncrement > 0)
				m_xScaleIncrement = -abs(m_xScaleIncrement);
			else if (m_spriteScale.X() <= 0.9f && m_xScaleIncrement < 0)
				m_xScaleIncrement = abs(m_xScaleIncrement);
		}
	}
	else
	{
		m_spriteScale.Set(1.f, 1.f);
	}
}

void Player::StopControl()
{
	m_moveDown = false;
	m_moveLeft = false;
	m_moveRight = false;
	m_moveUp = false;
	m_fireOn = false;
	m_usingSpecialAbility = false;
}

void Player::Draw() const
{
	Actor::Draw();
}

void Player::SpawnParticle()
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		const float particleSize = Utils::randomgen(5, 10);
		const float xOffset = Utils::randomgen(0, 40) - 20;
		const float yOffset = Utils::randomgen(0, 40) - 20 + 50;
		const float angle = Utils::randomgen(0, 360);
		const uint8_t r = Utils::randomgen(230, 255);
		const uint8_t g = Utils::randomgen(100, 200);
		const uint8_t b = Utils::randomgen(230, 255);
		const uint8_t a = 80;
		m_sharedContext->m_actorManager->AddParticle(new Particle(m_sharedContext, m_position.X() + xOffset, m_position.Y() + yOffset, particleSize, particleSize, angle, sf::Color(r, g, b, a), 1.f));
		m_particleSpawnTimer = 0;
	}
}

float Player::GetMaxLife() const { return m_maxLife; }
float Player::GetLife() const { return m_life; }

void Player::MakeInvulnerable()
{
	m_invulnerable = true;
}

bool Player::IsInvulnerable() const { return m_invulnerable; }

void Player::CheckControls()
{
	const bool reverseMovement = m_sharedContext->m_gameInfo->m_reverseMovement;

	if (!reverseMovement)
	{
		m_moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		m_moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		m_moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		m_moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	}
	else
	{
		m_moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		m_moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		m_moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		m_moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
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

void Player::UseSpecialAbility(EventDetails* l_details)
{
	if (!m_usingSpecialAbility && m_pushVelocity == 0 && m_velocity > 0)
	{
		AddImpulsion(m_direction, 1500, -10000);
		m_usingSpecialAbility = true;
	}
}

void Player::ReleaseSpecialAbility(EventDetails* l_details)
{
	m_usingSpecialAbility = false;
}

void Player::RemoveLife(const float p_value, const bool p_constantDamages, const Vector2D<float> p_direction)
{
	if (!IsInvulnerable())
	{
		AddImpulsion(p_direction, 600);
		m_life -= p_value;
		m_getHit = true;
		m_timeSinceLastHit = 0.f;
		if (!p_constantDamages)
			MakeInvulnerable();

		if (m_life <= 0)
		{
			m_life = 0;
			m_sharedContext->m_gameInfo->m_gameOver = true;
		}
	}
}

void Player::AddLife(const float p_value) const
{
	m_life += p_value;
	if (m_life >= m_maxLife)
		m_life = m_maxLife;
}
