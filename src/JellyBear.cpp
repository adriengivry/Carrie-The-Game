#include "JellyBear.h"
#include "StateManager.h"

JellyBear::JellyBear(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{
	SetTexture(__JELLYBEAR_TEXTURE);

	m_velocity = __JELLYBEAR_SPEED;
	m_maxLife = __JELLYBEAR_LIFE;
	m_damages = __JELLYBEAR_DAMAGES;

	m_life = m_maxLife;
}

JellyBear::~JellyBear()
{
}

void JellyBear::Update(const sf::Time & l_time)
{
	m_velocity = __JELLYBEAR_SPEED;

	Player* player = m_sharedContext->m_actorManager->GetPlayer();

	bool projectileFound = false;

	for (auto it : m_sharedContext->m_actorManager->GetProjectile()) 
	{
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(2000, 1));
		Utils::centerOrigin(rect);
		rect.setPosition(it->GetPosition().ToSFVector());
		rect.setRotation(it->GetDirection().GetAngle());

		sf::RectangleShape rect2;
		rect2.setSize(sf::Vector2f(2000, 1));
		Utils::centerOrigin(rect2);
		rect2.setPosition(m_position.ToSFVector());
		rect2.setRotation(90 + it->GetDirection().GetAngle());

		sf::RectangleShape rect3;

		//const float angle = m_position.AngleTo(it->GetPosition());

		/*if (angle < 5 && m_position.DistanceTo(it->GetPosition()) <= 250 && !it->MustDie())
		{
			projectileFound = true;
			SetTarget(it);
		}
		else
			SetTarget(m_sharedContext->m_actorManager->GetPlayer());*/
	}
	
	if (projectileFound)
		m_velocity *= 5;

	Enemy::Update(l_time);
}
