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
}

void Player::React(EventDetails* l_details)
{
	switch (l_details->m_keyCode)
	{
	case sf::Keyboard::Left:
		m_moveLeft = true;
		break;

	case sf::Keyboard::Right:
		m_moveRight = true;
		break;

	case sf::Keyboard::Up:
		m_moveUp = true;
		break;

	case sf::Keyboard::Down:
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
	case sf::Keyboard::Left:
		m_moveLeft = false;
		break;

	case sf::Keyboard::Right:
		m_moveRight = false;
		break;

	case sf::Keyboard::Up:
		m_moveUp = false;
		break;

	case sf::Keyboard::Down:
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

	m_direction = direction;

	Actor::Move(l_time);
}

void Player::StopControl()
{
	m_moveDown = false;
	m_moveLeft = false;
	m_moveRight = false;
	m_moveUp = false;
}


