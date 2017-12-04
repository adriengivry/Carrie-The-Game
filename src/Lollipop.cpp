#include "Lollipop.h"
#include "StateManager.h"

Lollipop::Lollipop(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y),
	m_isDodging(false)
{
	SetTexture(__LOLLIPOP_TEXTURE);

	float level = m_sharedContext->m_gameInfo->m_currentLevel;

	m_velocity	= __LOLLIPOP_SPEED;
	m_cooldown = __LOLLIPOP_COOLDOWN;

	if (m_maxLife > 150)
		m_maxLife = 150;
	else
		m_maxLife	= __LOLLIPOP_LIFE * level * 1.1f;

	if (m_damages > 16)
		m_damages = 16;
	else
		m_damages = __LOLLIPOP_DAMAGES * level * 1.05f;

	m_life		= m_maxLife;
	m_timer		= 0;

	m_followTarget		= false;
	m_damagesOnContact	= false;
}

Lollipop::~Lollipop() {}

void Lollipop::Update(const sf::Time & l_time)
{
	if (m_isDodging)
	{
		m_dodgeTimer += l_time.asSeconds();

		if (m_isDodging >= 0.1f)
		{
			m_isDodging = false;
			m_dodgeTimer = 0.0f;
			m_direction.Set(0, 0);
		}
	}

	if (!m_isDodging)
		Dodge();

	Enemy::Update(l_time);
}

void Lollipop::Attack()
{
	//Shoot();

	Enemy::Attack();
}

void Lollipop::Dodge()
{
	bool projectileFound = false;

	for (auto it : m_sharedContext->m_actorManager->GetProjectile())
	{
		if (it->IsFriendly())
		{
			const float angle = m_position.AngleTo(it->GetPosition());

			if (m_position.DistanceTo(it->GetPosition()) <= 250 && !it->MustDie())
			{
				projectileFound = true;

				if (angle < 90)
				{
					m_direction.X() = -it->GetDirection().Y();
					m_direction.Y() = it->GetDirection().X();
				}		
				else if (angle > 90)
				{
					m_direction.X() = it->GetDirection().Y();
					m_direction.Y() = -it->GetDirection().X();
				}

				m_direction.Normalize();
			}
		}	
	}

	if (projectileFound)
		m_isDodging = true;
}

void Lollipop::Shoot()
{
	Vector2D<float> projectileDirection;
	projectileDirection.Set(1, m_position.AngleTo(m_sharedContext->m_actorManager->GetPlayer()->GetPosition()), POLAR);

	Projectile* projectile = new Projectile(m_sharedContext, projectileDirection, m_position.X(), m_position.Y(), false, false);
	projectile->SetDamages(m_damages);
	projectile->SetSpeed(m_velocity * 7.f);

	m_sharedContext->m_actorManager->AddProjectile(projectile);

	StartCooldown();
}
