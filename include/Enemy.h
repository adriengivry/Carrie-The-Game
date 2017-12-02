#pragma once

#include "Actor.h"

class Enemy : public Actor
{
public:
	explicit Enemy(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Enemy();

	void SetTarget(Actor* p_target);

	void Kill();

	void RemoveLife(const float p_damages);

	void Update(const sf::Time& l_time) override;

	void Draw() const override;

	void DrawLifebar() const;

protected :
	Actor* m_target;

	float m_damages;

	float m_maxLife;
	float m_life;
};