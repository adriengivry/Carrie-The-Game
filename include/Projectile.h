#pragma once
#include "Actor.h"

class Projectile : public Actor
{
private:
	const std::string __PROJECTILE_TEXTURE = "Projectile";
	const float __PROJECTILE_SPEED = 800;
	const float __PROJECTILE_DAMAGES = 10;
	const float __PROJECTILE_HITRATE = 100;

public:
	explicit Projectile(SharedContext* p_sharedContext, const Vector2D<float> p_direction, const float p_x = 0, const float p_y = 0);
	~Projectile();

	void Update(const sf::Time& l_time) override;

	void MultiplyDamages(const float p_value);
	void MultiplySpeed(const float p_value);
	void MultiplyHitrate(const float p_value);

private:
	float m_damages;
	float m_hitrate;
};
