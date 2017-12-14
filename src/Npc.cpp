#include "Npc.h"
#include "StateManager.h"


Npc::Npc(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	SetTexture(__NPC_TEXTURE_2);
	Desactivate();

	m_questionMarkRotationInc = 30;

	m_talkingTimer = 0;
	m_currentFrame = 0;
	m_talkingFrameDuration = 0.1f;

	m_speachTimer = 0;
	m_speachDuration = 2.0f;

	m_answer = false;
	m_question = static_cast<QuestionType>(0);

	if (m_sharedContext->m_textureManager->RequireResource("Bubble"))
	{
		m_speachBubble.setTexture(*m_sharedContext->m_textureManager->GetResource("Bubble"));
		m_speachBubble.setPosition(1100, 100);
		Utils::centerOrigin(m_speachBubble);
	}

	if (m_sharedContext->m_textureManager->RequireResource("Question_Mark"))
	{
		m_questionMark.setTexture(*m_sharedContext->m_textureManager->GetResource("Question_Mark"));
		m_questionMark.setPosition(1020, 115);
		Utils::centerOrigin(m_questionMark);
	}

	m_talking = false;
}

Npc::~Npc()
{
}

void Npc::Activate()
{
	if (!m_isActive)
	{
		m_isActive = true;
		GenerateQuestion();
	}
}

void Npc::Desactivate()
{
	if (m_isActive)
	{
		m_isActive = false;
	}
}

void Npc::GenerateQuestion()
{
	m_question = static_cast<QuestionType>(Utils::randomgen(0, 3));
	m_answer = static_cast<bool>(Utils::randomgen(0, 1));

	uint16_t goodValue = 0;

	switch (m_question)
	{
	case QuestionType::TRAVEL:
		goodValue = m_sharedContext->m_gameInfo->m_travelledDistance;
		break;

	case QuestionType::ENEMIES_SPAWNED:
		goodValue = m_sharedContext->m_gameInfo->m_spawnedEnemies;
		break;

	case QuestionType::PROJECTILE_SPAWNED:
		goodValue = m_sharedContext->m_gameInfo->m_spawnedProjectiles;
		break;

	case QuestionType::LEVEL_DURATION:
		goodValue = m_sharedContext->m_gameInfo->m_levelDuration;
		break;

	case QuestionType::SPAWNPOINTS_SPAWNED:
		goodValue = m_sharedContext->m_gameInfo->m_spawnedSpawnPoints;
		break;
	}

	if (goodValue == 0)
		m_randomValue = 1;
	else
	{
		do m_randomValue = Utils::randomgen(goodValue - goodValue / 1.5f, goodValue + goodValue / 1.5f);
		while (m_randomValue == goodValue);
	}
	
}

bool Npc::IsTalking() const { return m_talking; }
bool Npc::IsActive() const { return m_isActive; }
bool Npc::GetAnswer() const { return m_answer; }

void Npc::Update(const sf::Time& l_time)
{
	if (m_position.DistanceTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()) <= __TALK_DISTANCE && m_isActive)
	{
		m_talking = true;
		m_sharedContext->m_gameInfo->m_questionAsked = true;
	}

	if (m_isActive)
	{
		Actor::Update(l_time);
		if (m_talking)
		{
			if (m_speachTimer <= m_speachDuration)
			{
				m_speachTimer += l_time.asSeconds();
				m_talkingTimer += l_time.asSeconds();
				if (m_talkingTimer >= m_talkingFrameDuration)
				{
					m_talkingTimer = 0;
					if (m_currentFrame == 0)
					{
						SetTexture(__NPC_TEXTURE_2);
						m_currentFrame = 1;
					}
					else if (m_currentFrame == 1)
					{
						SetTexture(__NPC_TEXTURE_1);
						m_currentFrame = 0;
					}
				}
			}
			else
			{
				SetTexture(__NPC_TEXTURE_2);
			}
		}
		else
		{
			SetTexture(__NPC_TEXTURE_2);

			m_questionMark.rotate(m_questionMarkRotationInc * l_time.asSeconds());

			const uint16_t rotation = m_questionMark.getRotation();

			if (rotation >= 5 && rotation < 180 && m_questionMarkRotationInc > 0)
				m_questionMarkRotationInc = -abs(m_questionMarkRotationInc);
			else if (rotation <= 355 && rotation >= 180 && m_questionMarkRotationInc < 0)
				m_questionMarkRotationInc = abs(m_questionMarkRotationInc);
		}
	}
}

void Npc::Draw() const
{
	if (m_isActive)
	{
		Actor::Draw();

		if (m_talking || m_sharedContext->m_gameInfo->m_questionAsked)
			DrawAffirmation();
		else if (!m_sharedContext->m_gameInfo->m_questionAsked)
			m_sharedContext->m_wind->GetRenderWindow()->draw(m_questionMark);
	}
}

void Npc::DrawAffirmation() const
{
	sf::Text question;
	if (m_sharedContext->m_fontManager->RequireResource("Retro"))
		question.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
	question.setCharacterSize(22);
	question.setPosition(1100, 80);
	question.setFillColor(sf::Color::Black);

	std::string value;

	switch (m_question)
	{
	case QuestionType::TRAVEL:
		if (m_answer)
			value = std::to_string(static_cast<int>(m_sharedContext->m_gameInfo->m_travelledDistance));
		else
			value = std::to_string(m_randomValue);

		question.setString("Wow ! You travelled \n" + value + " meters !");
		break;

	case QuestionType::ENEMIES_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedEnemies);
		else
			value = std::to_string(m_randomValue);

		question.setString("Seems that " + value + " enemies\nspawned here !");
		break;

	case QuestionType::PROJECTILE_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedProjectiles);
		else
			value = std::to_string(m_randomValue);

		question.setString("Damn it ! You threw \n" + value + " pieces of chocolate !");
		break;

	case QuestionType::LEVEL_DURATION:
		if (m_answer)
			value = std::to_string(static_cast<uint16_t>(m_sharedContext->m_gameInfo->m_levelDuration));
		else
			value = std::to_string(m_randomValue);

		question.setString("My watch is saying\nthat you took " + value + " seconds\nto clean this!");
		break;

	case QuestionType::SPAWNPOINTS_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedSpawnPoints);
		else
			value = std::to_string(m_randomValue);

		question.setString("I have counted " + value + " donuts\nin this level !");
		break;
	}

	Utils::centerOrigin(question);

	m_sharedContext->m_wind->GetRenderWindow()->draw(m_speachBubble);
	m_sharedContext->m_wind->GetRenderWindow()->draw(question);
}
