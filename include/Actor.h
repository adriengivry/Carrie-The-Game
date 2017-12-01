#pragma once

#include <SFML/Graphics.hpp>

#include "SharedContext.h"
#include "Vector2D.h"

using namespace AGMath;

class Actor
{
	const float __ACTOR_DEFAULT_VELOCITY = 250;
	const std::string __ACTOR_TEXTURE = "Actor";

public:
	explicit Actor(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	virtual ~Actor();

	virtual void Update(const sf::Time& l_time);
	virtual void Move(const sf::Time& l_time);
	void Draw() const;

	bool IsIntersecting(Actor* p_otherActor) const;
	bool MustDie() const;

	Vector2D<float>& GetPosition();
	Vector2D<float>& GetDirection();
	sf::Sprite& GetSprite();

	void SetVelocity(const float p_newVelocity);
	void SetDirection(const float p_x, const float p_y);
	void SetDirection(const float p_angle);
	void SetPosition(const float p_x, const float p_y);	
	void SetTexture(const std::string p_textureName);

protected:
	bool m_textureGetSet;

	sf::Sprite m_sprite;

	SharedContext* m_sharedContext;

	Vector2D<float> m_position;
	Vector2D<float> m_direction;
	float m_velocity;

	bool m_orientable;
	bool m_mustDie;
};