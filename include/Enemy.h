#pragma once

#include "Actor.h"

class Enemy : public Actor
{
	const std::string	__ENEMY_TEXTURE	= "Enemy";
	const float			__ENEMY_SPEED	= 300;

public:
	explicit Enemy(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Enemy();

	void SetTarget(Actor* p_target);

	void Update(const sf::Time& l_time) override;

private:
	Actor* m_target;
};