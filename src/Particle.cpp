#include "Particle.h"

Particle::Particle(SharedContext* p_sharedContext, const float p_x, const float p_y, const float p_sizeX,
	const float p_sizeY, const float p_angle, const sf::Color p_color, const float p_lifetime) :
	m_sharedContext(p_sharedContext),
	m_timeSinceCreation(0),
	m_lifetime(p_lifetime), 
	m_defaultOpacity(p_color.a)
{
	m_rect.setPosition(p_x, p_y);
	m_rect.setSize(sf::Vector2f(p_sizeX, p_sizeY));
	m_rect.setFillColor(p_color);
	Utils::centerOrigin(m_rect);
	m_rect.setRotation(p_angle);
}

bool Particle::MustDie() const
{
	return m_timeSinceCreation >= m_lifetime;
}

void Particle::Update(const sf::Time& l_time)
{
	m_timeSinceCreation += l_time.asSeconds();

	if (m_lifetime - m_timeSinceCreation <= m_lifetime / 2)
	{
		sf::Color previousColor = m_rect.getFillColor();
		previousColor.a = m_defaultOpacity * (m_lifetime - m_timeSinceCreation);
		m_rect.setFillColor(previousColor);
	}
}

void Particle::Draw() const
{
	m_sharedContext->m_wind->GetRenderWindow()->draw(m_rect);
}
