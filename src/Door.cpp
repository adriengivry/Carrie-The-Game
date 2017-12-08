#include "Door.h"
#include "StateManager.h"

Door::Door(SharedContext* p_sharedContext, const float p_x, const float p_y, const bool p_answer)
	: Actor(p_sharedContext, p_x, p_y)
{
	Desactivate();

	if (m_sharedContext->m_gameInfo->m_mapType == MapType::MAP1)
		SetTexture(__MAP_1_DOOR_TEXTURE);
	else if (m_sharedContext->m_gameInfo->m_mapType == MapType::MAP2)
		SetTexture(__MAP_2_DOOR_TEXTURE);

	m_answer = p_answer;

	m_alreadyGetUsed = false;

	m_gotAShadow = false;

	m_velocity = 0.0f;
}

Door::~Door()
{
}

void Door::Activate()
{
	m_activated = true;
}

void Door::Desactivate()
{
	m_activated = false;
}

void Door::Use() const
{
	m_sharedContext->m_audioManager->Pause("Game");
	if (m_answer == m_sharedContext->m_actorManager->GetNpc()->GetAnswer())
	{
		m_sharedContext->m_gameInfo->m_getCursed = false;
		m_sharedContext->m_gameInfo->m_doorPassed = true;
		++m_sharedContext->m_gameInfo->m_currentLevel;
		m_sharedContext->m_audioManager->PlaySound("Truth");
	}
	else
	{
		m_sharedContext->m_gameInfo->m_doorPassed = true;
		m_sharedContext->m_gameInfo->m_getCursed = true;
		++m_sharedContext->m_gameInfo->m_currentLevel;
		m_sharedContext->m_audioManager->PlaySound("Curse");
		SelectCurse();
	}
}

void Door::Draw() const
{
	Actor::Draw();

	if (m_sharedContext->m_gameInfo->m_questionAsked)
		DrawLabel();
}

void Door::DrawLabel() const
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(0, 0, 0, 125));
	rect.setSize(sf::Vector2f(300, 50));

	sf::Text label;
	label.setCharacterSize(35);
	label.setFillColor(sf::Color::White);
	if (m_sharedContext->m_fontManager->RequireResource("Retro"))
		label.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));

	sf::Vector2f labelPos = m_position.ToSFVector();
	
	labelPos.y += 130.0f;

	label.setPosition(labelPos);
	if (m_answer)
	{
		label.setFillColor(sf::Color::White);
		label.setString("YES IT'S TRUE");
	}
	else
	{
		label.setFillColor(sf::Color::White);
		label.setString("GO TO HELL LIAR!");
	}

	rect.setPosition(label.getPosition());

	Utils::centerOrigin(rect);
	Utils::centerOrigin(label);

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
	m_sharedContext->m_wind->GetRenderWindow()->draw(label);
}

bool Door::IsActivated() const { return m_activated; }

void Door::SelectCurse() const
{
	GameInfo* gameInfo = m_sharedContext->m_gameInfo;

	uint8_t curse;
		
	do
	{
		curse = Utils::randomgen(0, 4);
		if (curse == 0)
			if (Utils::randomgen(0, 2) != 0)
				curse = Utils::randomgen(0, 4);
	}

	while (curse == REVERSE_MOVEMENT && gameInfo->m_reverseMovement);

	switch (curse)
	{
	case REVERSE_MOVEMENT:
		gameInfo->m_reverseMovement = true;
		break;

	case SLOWER_CARRIE:
		++gameInfo->m_slowerCarrie;
		break;

	case SLOWER_PROJECTILES:
		++gameInfo->m_slowerProjectiles;
		break;

	case WEAKER_PROJECTILES:
		++gameInfo->m_weakerProjectiles;
		break;

	case REDUCED_PRECISION:
		++gameInfo->m_reducedPrecision;
		break;

	default:
		break;
	}

	gameInfo->m_curseType = curse;
}

void Door::Update(const sf::Time& l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_sharedContext->m_gameInfo->m_questionAsked && m_position.DistanceTo(player->GetPosition()) <= __DOOR_ACTIVATION_ZONE && IsActivated() && player->GetDirection().Y() < 0 && !m_alreadyGetUsed)
	{
		Use();
		m_alreadyGetUsed = true;
	}

	Actor::Update(l_time);
}
