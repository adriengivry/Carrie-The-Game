#include "Npc.h"
#include "StateManager.h"


Npc::Npc(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	SetTexture(__NPC_TEXTURE);
	Desactivate();

	m_talking = false;
}

Npc::~Npc()
{
}

void Npc::Activate()
{
	m_isActive = true;
}

void Npc::Desactivate()
{
	m_isActive = false;
}

bool Npc::IsTalking() const { return m_talking; }
bool Npc::IsActive() const { return m_isActive; }

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

	m_sharedContext->m_wind->GetRenderWindow()->draw(rect);
}
