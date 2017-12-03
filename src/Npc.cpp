#include "Npc.h"
#include "StateManager.h"


Npc::Npc(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	SetTexture(__NPC_TEXTURE);
	Desactivate();

	m_answer = false;
	m_question = static_cast<QuestionType>(0);

	if (m_sharedContext->m_textureManager->RequireResource("Bubble"))
	{
		m_speachBubble.setTexture(*m_sharedContext->m_textureManager->GetResource("Bubble"));
		m_speachBubble.setPosition(1150, 90);
		m_speachBubble.setScale(0.6f, 0.6f);
		Utils::centerOrigin(m_speachBubble);
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
	m_question = static_cast<QuestionType>(Utils::randomgen(0, 2));
	const uint8_t answer = Utils::randomgen(1, 100);

	if (answer < 50)
		m_answer = true;
	else
		m_answer = false;

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
	}

	if (goodValue == 0)
		m_randomValue = 1;
	else
	{
		do m_randomValue = Utils::randomgen(goodValue - goodValue / 2, goodValue + goodValue / 2);
		while (m_randomValue == goodValue);
	}
	
}

bool Npc::IsTalking() const { return m_talking; }
bool Npc::IsActive() const { return m_isActive; }
bool Npc::GetAnswer() const { return m_answer; }

void Npc::Update(const sf::Time& l_time)
{
	m_talking = false;

	if (m_isActive)
		Actor::Update(l_time);

	if (m_position.DistanceTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()) <= __TALK_DISTANCE)
		m_talking = true;
}

void Npc::Draw() const
{
	if (m_isActive)
	{
		Actor::Draw();

		if (m_talking)
			DrawAffirmation();
	}
}

void Npc::DrawAffirmation() const
{
	sf::Text question;
	question.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
	question.setCharacterSize(20);
	question.setPosition(1150, 70);
	question.setFillColor(sf::Color::Black);

	std::string value;

	switch (m_question)
	{
	case QuestionType::TRAVEL:
		if (m_answer)
			value = std::to_string(static_cast<int>(m_sharedContext->m_gameInfo->m_travelledDistance));
		else
			value = std::to_string(m_randomValue);

		question.setString("Wow ! You travelled \n" + value + " pixels !");
		break;

	case QuestionType::ENEMIES_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedEnemies);
		else
			value = std::to_string(m_randomValue);

		question.setString("I have counted that \n" + value + " enemies spawned !");
		break;

	case QuestionType::PROJECTILE_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedProjectiles);
		else
			value = std::to_string(m_randomValue);

		question.setString("Ahahah, you threw \n" + value + " pieces of chocolate !");
		break;
	}

	Utils::centerOrigin(question);

	m_sharedContext->m_wind->GetRenderWindow()->draw(m_speachBubble);
	m_sharedContext->m_wind->GetRenderWindow()->draw(question);
}
