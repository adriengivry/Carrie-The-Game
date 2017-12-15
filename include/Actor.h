#pragma once

#include <SFML/Graphics.hpp>

#include "SharedContext.h"
#include "Vector2D.h"

using namespace AGMath;

class Actor
{
protected:
	const float			__ACTOR_DEFAULT_VELOCITY	= 250;
	const std::string	__ACTOR_TEXTURE				= "Actor";
	const float			__ACTOR_HITBOX_SCALE		= 0.6f;

public:
	explicit Actor(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	virtual ~Actor();

	virtual void Update(const sf::Time& l_time);
	virtual void Move(const sf::Time& l_time);
	virtual void Draw() const;

	virtual void SpawnParticle();

	bool IsIntersecting(Actor* p_otherActor) const;
	bool MustDie() const;

	void AddImpulsion(const Vector2D<float> p_direction, float p_power = 300);

	float GetVelocity() const;
	Vector2D<float>& GetPosition();
	Vector2D<float>& GetDirection();
	sf::Sprite& GetSprite();
	sf::RectangleShape& GetCollider();

	uint16_t& GetZBuffer();

	void SetVelocity(const float p_newVelocity);
	void SetDirection(const float p_x, const float p_y);
	void SetDirection(const float p_angle);
	void SetPosition(const float p_x, const float p_y);	
	void SetTexture(const std::string p_textureName);

protected:
	bool m_textureGetSet;
	Vector2D<float> m_spriteScale;

	bool m_flippable;

	sf::Sprite m_sprite;
	sf::RectangleShape m_collider;

	SharedContext* m_sharedContext;

	Vector2D<float> m_position;
	Vector2D<float> m_direction;

	uint16_t m_zBuffer;

	float m_velocity;
	float m_maxVelocity;
	float m_acceleration;
	float m_velocityMultiplicator;

	bool m_orientable;
	bool m_mustDie;

	float m_lifetimeCounter;

	float m_pushMaxVelocity;
	float m_pushVelocity;
	float m_pushAcceleration;
	Vector2D<float> m_pushDirection;

	float m_particleSpawnTimer;
	float m_particleSpawnDelay;
};