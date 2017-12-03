#include "Npc.h"
#include "StateManager.h"


Npc::Npc(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	SetTexture(__NPC_TEXTURE);
	Desactivate();

	m_answer = false;
	m_question = static_cast<QuestionType>(0);

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

	switch (m_question)
	{
	case QuestionType::TRAVEL:
		do m_randomValue = Utils::randomgen(6000, 7000);
		while (m_randomValue == m_sharedContext->m_gameInfo->m_travelledDistance);

		break;

	case QuestionType::ENEMIES_SPAWNED:
		do m_randomValue = Utils::randomgen(30, 50);
		while (m_randomValue == m_sharedContext->m_gameInfo->m_spawnedEnemies);
		break;

	case QuestionType::PROJECTILE_SPAWNED:
		do m_randomValue = Utils::randomgen(50, 100);
		while (m_randomValue == m_sharedContext->m_gameInfo->m_spawnedProjectiles);
		break;
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
	sf::RectangleShape rect;

	rect.setSize(sf::Vector2f(1920, 1080));
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(0, 0, 0, 150));

	sf::Text question;
	question.setFont(*m_sharedContext->m_fontManager->GetResource("Retro"));
	question.setCharacterSize(25);
	question.setPosition(m_sharedContext->m_wind->GetWindowCenter().x, m_sharedContext->m_wind->GetWindowCenter().y);

	std::string value;

	switch (m_question)
	{
	case QuestionType::TRAVEL:
		if (m_answer)
			value = std::to_string(static_cast<int>(m_sharedContext->m_gameInfo->m_travelledDistance));
		else
			value = std::to_string(m_randomValue);

		question.setString("Wow ! You travelled " + value + " pixels !");
		break;

	case QuestionType::ENEMIES_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedEnemies);
		else
			value = std::to_string(m_randomValue);

		question.setString("I have counted that " + value + " enemies spawned !");
		break;

	case QuestionType::PROJECTILE_SPAWNED:
		if (m_answer)
			value = std::to_string(m_sharedContext->m_gameInfo->m_spawnedProjectiles);
		else
			value = std::to_string(m_randomValue);

		question.setString("Ahahah, you threw " + value + " pieces of chocolate !");
		break;
	}

	Utils::centerOrigin(question);

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
	m_sharedContext->m_wind->GetRenderWindow()->draw(question);
}
