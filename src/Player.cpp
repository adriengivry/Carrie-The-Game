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

	m_damagesMultiplicator = 1;
	m_projectileSpeedMultiplicator = 1;
	m_hitrateMultiplicator = 1;

	m_collide = false;

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
	switch (l_details->m_keyCode)
	{
	case sf::Keyboard::A:
		m_moveLeft = true;
		break;

	case sf::Keyboard::D:
		m_moveRight = true;
		break;

	case sf::Keyboard::W:
		m_moveUp = true;
		break;

	case sf::Keyboard::S:
		m_moveDown = true;
		break;

	default:
		break;
	}
}

void Player::Unreact(EventDetails* l_details)
{
	switch (l_details->m_keyCode)
	{
	case sf::Keyboard::A:
		m_moveLeft = false;
		break;

	case sf::Keyboard::D:
		m_moveRight = false;
		break;

	case sf::Keyboard::W:
		m_moveUp = false;
		break;

	case sf::Keyboard::S:
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

	Actor::Move(l_time);

	m_collide = false;

	if (m_position.X() < 320)
	{
		m_position.X(320);
		m_collide = true;
	}

	if (m_position.X() > 1600)
	{
		m_position.X(1600);
		m_collide = true;
	}

	if (m_position.Y() < 120)
	{
		m_position.Y(120);
		m_collide = true;
	}

	if (m_position.Y() > 780)
	{
		m_position.Y(780);
		m_collide = true;
	}
}

void Player::Update(const sf::Time& l_time)
{
	Actor::Update(l_time);

	for (auto otherActor : m_sharedContext->m_actorManager->GetEnemies())
		if (IsIntersecting(otherActor))
			m_sharedContext->m_gameInfo->m_gameOver = true;
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

void Player::Fire(EventDetails* l_details)
{
	if (m_sharedContext->m_actorManager->GetNpc()->IsTalking())
		return;

	Vector2D<float> mousePos;
	mousePos.Set(l_details->m_mouse.x, l_details->m_mouse.y);

	Vector2D<float> projectileDirection;
	projectileDirection.Set(1, m_position.AngleTo(mousePos), POLAR);

	Projectile* newProjectile = new Projectile(m_sharedContext, projectileDirection, m_position.X(), m_position.Y());
	newProjectile->MultiplyDamages(m_damagesMultiplicator);
	newProjectile->MultiplyHitrate(m_hitrateMultiplicator);
	newProjectile->MultiplySpeed(m_projectileSpeedMultiplicator);

	m_sharedContext->m_actorManager->AddProjectile(newProjectile);
}

void Player::RemoveLife(const float p_value)
{
	m_life -= p_value;
	if (m_life <= 0)
	{
		m_life = 0;
		m_sharedContext->m_gameInfo->m_gameOver = true;
	}
}


