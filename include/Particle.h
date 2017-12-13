#pragma once
#include "SharedContext.h"
#include "Vector2D.h"

using namespace AGMath;

class Particle
{
public:
	Particle(SharedContext* p_sharedContext, const float p_x, const float p_y, const float p_sizeX, const float p_sizeY, const float p_angle, const sf::Color p_color, const float p_lifetime);
	~Particle() {}

	bool MustDie() const;

	void Update(const sf::Time& l_time);

	void Draw() const;

private:

	sf::RectangleShape m_rect;

	float m_timeSinceCreation;
	float m_lifetime;

	float m_defaultOpacity;

	SharedContext* m_sharedContext;
};
