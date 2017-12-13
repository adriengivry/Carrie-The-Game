#include "Jelly.h"
#include "StateManager.h"

Jelly::Jelly(SharedContext * p_sharedContext, const float p_x, const float p_y) :
	Enemy(p_sharedContext, p_x, p_y)
{

	SetTexture(__JELLY_TEXTURE);
	m_spriteScale.Set(1.2f, 1.2f);
	
	Jelly::GenerateStats();
	ResetLife();
}

void Jelly::SpawnParticle()
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		const float particleSize = Utils::randomgen(5, 10);
		const float xOffset = Utils::randomgen(0, 20) - 10;
		const float yOffset = Utils::randomgen(0, 20) - 10;
		const float angle = Utils::randomgen(0, 360);
		const uint8_t r = Utils::randomgen(160, 185);
		const uint8_t g = Utils::randomgen(35, 54);
		const uint8_t b = Utils::randomgen(80, 100);
		const uint8_t a = 80;
		m_sharedContext->m_actorManager->AddParticle(new Particle(m_sharedContext, m_position.X() + xOffset, m_position.Y() + m_shadowOffset + yOffset, particleSize, particleSize, angle, sf::Color(r, g, b, a), 2.f));
		m_particleSpawnTimer = 0;
	}
}

void Jelly::OnDeath()
{
	m_sharedContext->m_audioManager->PlaySound("Death_Jelly");
	m_sharedContext->m_gameInfo->m_toothPaste += 1;
}

void Jelly::GenerateStats()
{
	m_velocity = __JELLY_SPEED;
	m_maxVelocity = m_velocity;

	m_maxLife = CalculateStat(__JELLY_LIFE, __JELLY_LIFE_INCREMENTATION_COEFFICIENT, __JELLY_MAX_LIFE);
	m_damages = CalculateStat(__JELLY_DAMAGES, __JELLY_DAMAGES_INCREMENTATION_COEFFICIENT, __JELLY_MAX_DAMAGES);
}
