#pragma once

#include "Actor.h"

enum class SpawnerType
{
	JELLY_SPAWNER = 0,
	JELLYBEAR_SPAWNER,
	LOLLIPOP_SPAWNER,
	CAKEMONSTER_SPAWNER,
	CROCODILE_SPAWNER
};

class SpawnPoint : public Actor
{
	const std::string	__SPAWNPOINT_TEXTURE = "SpawnPoint";
	const int			__SPAWNPOINT_MAXSPAWN = 7;
	const float			__SPAWNPOINT_SPAWN_FREQUENCY = 1.0f;
	const float			__DISTANCE_MIN_TO_PLAYER = 400.0f;
	const float			__DISTANCE_MIN_TO_OTHER_SPAWNPOINT = 150.0f;

public:
	explicit SpawnPoint(SharedContext* p_sharedContext, const float p_secondBeforeActivation = 0);
	~SpawnPoint();
	
	bool IsDone() const;
	bool IsActive() const;

	void Desactivate();
	void Activate();
	void ToggleActive();

	void SpawnEnemy();

	void Update(const sf::Time& l_time) override;
	void Draw() const override;

private:
	Actor* m_target;

	int m_maxSpawn;

	bool m_active;

	float m_spawnFrequency;
	float m_timer;

	float m_secondsBeforeActivation;

	float m_activationTimer;

	SpawnerType m_type;

};