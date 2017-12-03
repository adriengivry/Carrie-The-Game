#pragma once

#include "Enemy.h"

class CakeMonster : public Enemy
{
	const std::string	__CAKEMONSTER_TEXTURE = "CakeMonsterFront";
	const float			__CAKEMONSTER_SPEED = 300;
	const float			__CAKEMONSTER_LIFE = 1;
	const float			__CAKEMONSTER_DAMAGES = 8;
	const float			__CAKEMONSTER_COOLDOWN = 0.5;

public:
	explicit CakeMonster(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~CakeMonster();

	void Update(const sf::Time& l_time) override;

	void Shoot();

private:
	float m_timer;

};