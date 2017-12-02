#include "Door.h"
#include "StateManager.h"

Door::Door(SharedContext* p_sharedContext, const float p_x, const float p_y)
	: Actor(p_sharedContext, p_x, p_y)
{
	Desactivate();

	SetTexture(__DOOR_TEXTURE);

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

void Door::Use()
{
	exit(0);
}

bool Door::IsActivated() const { return m_activated; }

void Door::Update(const sf::Time& l_time)
{
	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	if (m_position.DistanceTo(player->GetPosition()) <= __DOOR_ACTIVATION_ZONE && IsActivated() && player->GetDirection().Y() < 0)
	{
		Use();
	}

	Actor::Update(l_time);
}
