#pragma once
#include "Actor.h"

enum class ProjectileType
{
	NORMAL,
	LASER,
};

class Projectile : public Actor
{
private:
	const float __PROJECTILE_SPEED = 800;
	const float __PROJECTILE_DAMAGES = 50;
	const float __PROJECTILE_HITRATE = 100;

public:
	explicit Projectile(SharedContext* p_sharedContext, const Vector2D<float> p_direction, 
		Actor* p_source, 
		const float p_x = 0, const float p_y = 0, 
		const ProjectileType p_projectileType = ProjectileType::NORMAL,
		const std::string p_texture = "Chocolate", 
		const float p_scaleX = 1.f, const float p_scaleY = 1.f);

	~Projectile();

	void Update(const sf::Time& l_time) override;

	void MultiplyDamages(const float p_value);
	void MultiplySpeed(const float p_value);

	void Kill();

	Actor* GetSource() const;

	bool IsFriendly() const;
	bool DealsConstantDamages() const;

	void SetDamages(const float p_value);
	void SetSpeed(const float p_speed);
	void SetConstantlyRotate(const bool p_state);

	void SetColor(const sf::Color p_color);

private:
	bool m_constantlyRotate;

	ProjectileType m_projectileType;

	Actor* m_source;

	float m_damages;
};
