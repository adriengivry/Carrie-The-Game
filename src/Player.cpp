#include "Player.h"
#include "StateManager.h"

Player::Player(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Left", &Player::React, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Right", &Player::React, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Up", &Player::React, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Down", &Player::React, this);

	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Left_Stop", &Player::Unreact, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Right_Stop", &Player::Unreact, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Up_Stop", &Player::Unreact, this);
	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Move_Down_Stop", &Player::Unreact, this);

	m_sharedContext->m_eventManager->AddCallback(StateType::Game, "Fire", &Player::Fire, this);

	m_orientable = false;
	m_velocity = __PLAYER_SPEED;

	m_maxLife = __PLAYER_LIFE;
	m_life = m_maxLife;

	m_invulnerable = false;
	m_invulnerableTimer = 0;

	m_collide = false;

	m_sprite.setScale(0.7f, 0.7f);
	m_shadowScale.Set(0.8f, 0.8f);

	SetTexture(__PLAYER_TEXTURE);
}

Player::~Player()
{
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Left");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Right");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Up");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Down");

	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Left_Stop");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Right_Stop");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Up_Stop");
	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Move_Down_Stop");

	m_sharedContext->m_eventManager->RemoveCallback(StateType::Game, "Fire");
}

void Player::React(EventDetails* l_details)
{
	const bool reverseMovement = m_sharedContext->m_gameInfo->m_reverseMovement;

	switch (l_details->m_keyCode)
	{
	case sf::Keyboard::A:
		if (reverseMovement)
			m_moveRight = true;
		else
			m_moveLeft = true;
		break;

	case sf::Keyboard::D:
		if (reverseMovement)
			m_moveLeft = true;
		else
			m_moveRight = true;
		break;

	case sf::Keyboard::W:
		if (reverseMovement)
			m_moveDown = true;
		else
			m_moveUp = true;
		break;

	case sf::Keyboard::S:
		if (reverseMovement)
			m_moveUp = true;
		else
			m_moveDown = true;
		break;

	default:
		break;
	}
}

void Player::Unreact(EventDetails* l_details)
{
	const bool reverseMovement = m_sharedContext->m_gameInfo->m_reverseMovement;

	switch (l_details->m_keyCode)
	{
	case sf::Keyboard::A:
		if (reverseMovement)
			m_moveRight = false;
		else
			m_moveLeft = false;
		break;

	case sf::Keyboard::D:
		if (reverseMovement)
			m_moveLeft = false;
		else
			m_moveRight = false;
		break;

	case sf::Keyboard::W:
		if (reverseMovement)
			m_moveDown = false;
		else
			m_moveUp = false;
		break;

	case sf::Keyboard::S:
		if (reverseMovement)
			m_moveUp = false;
		else
			m_moveDown = false;
		break;

	default:
		break;
	}
}

void Player::Move(const sf::Time& l_time)
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

	if (direction.X() > 0 && direction.Y() == 0)
		SetTexture("Right");
	else if (direction.X() < 0 && direction.Y() == 0)
		SetTexture("Left");
	else if (direction.X() == 0 && direction.Y() > 0)
		SetTexture("Front");
	else if (direction.X() == 0 && direction.Y() < 0)
		SetTexture("Back");
	else if (direction.X() > 0 && direction.Y() > 0)
		SetTexture("Front_Right");
	else if(direction.X() < 0 && direction.Y() > 0)
		SetTexture("Front_Left");
	else if (direction.X() > 0 && direction.Y() < 0)
		SetTexture("Back_Right");
	if (direction.X() < 0 && direction.Y() < 0)
		SetTexture("Back_Left");

	m_direction = direction;

	if (!m_sharedContext->m_gameInfo->m_levelCompleted)
	{
		m_sharedContext->m_gameInfo->m_travelledDistance += abs(m_direction.X() * m_velocity * l_time.asSeconds());
		m_sharedContext->m_gameInfo->m_travelledDistance += abs(m_direction.Y() * m_velocity * l_time.asSeconds());
	}

	Actor::Move(l_time);

	m_collide = false;

	if (m_position.X() < 150)
	{
		m_position.X(150);
		m_collide = true;
	}

	if (m_position.X() > 1770)
	{
		m_position.X(1770);
		m_collide = true;
	}

	if (m_position.Y() < 250)
	{
		m_position.Y(250);
		m_collide = true;
	}

	if (m_position.Y() > 920)
	{
		m_position.Y(920);
		m_collide = true;
	}
}

void Player::Update(const sf::Time& l_time)
{
	Actor::Update(l_time);

	m_velocity = __PLAYER_SPEED * pow(m_sharedContext->m_gameInfo->__CARRIE_SLOW_MULTIPLICATOR, m_sharedContext->m_gameInfo->m_slowerCarrie);

	if (IsInvulnerable())
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 200));
		m_invulnerableTimer += l_time.asSeconds();
		if (m_invulnerableTimer >= __INVULNERABLE_DURATION)
			m_invulnerable = false;
	}
	else
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
	}
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

void Player::Fire(EventDetails* l_details)
{
	GameInfo* gameInfo = m_sharedContext->m_gameInfo;

	if (gameInfo->m_levelCompleted)
		return;

	Vector2D<float> mousePos;
	mousePos.Set(l_details->m_mouse.x, l_details->m_mouse.y);

	Vector2D<float> projectileDirection;
	projectileDirection.Set(1, m_position.AngleTo(mousePos), POLAR);

	Projectile* newProjectile = new Projectile(m_sharedContext, projectileDirection, m_position.X(), m_position.Y());

	newProjectile->MultiplyDamages(pow(gameInfo->__PROJECTILE_WEAK_MULTIPLICATOR, gameInfo->m_weakerProjectiles));
	newProjectile->MultiplySpeed(pow(gameInfo->__PROJECTILE_SLOW_MULTIPLICATOR, gameInfo->m_slowerProjectiles));

	m_sharedContext->m_actorManager->AddProjectile(newProjectile);
}

void Player::RemoveLife(const float p_value)
{
	if (!IsInvulnerable())
	{
		m_life -= p_value;

		if (m_life <= 0)
		{
			m_life = 0;
			m_sharedContext->m_gameInfo->m_gameOver = true;
		}
		else
		{
			MakeInvulnerable();
		}
	}
}


