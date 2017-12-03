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
	const float			__DISTANCE_MIN_TO_PLAYER = 400.0f;
	const float			__DISTANCE_MIN_TO_OTHER_SPAWNPOINT = 150.0f;
	//Jelly
	const int			__SPAWNPOINT_JELLY_MAXSPAWN = 5;
	const float			__SPAWNPOINT_JELLY_SPAWN_FREQUENCY = 2.0f;
	const float			__SPAWNPOINT_JELLY_WAIT = 1.0f;
	//JellyBear
	const int			__SPAWNPOINT_JELLYBEAR_MAXSPAWN = 3;
	const float			__SPAWNPOINT_JELLYBEAR_SPAWN_FREQUENCY = 7.0f;
	const float			__SPAWNPOINT_JELLYBEAR_WAIT = 2.0f;
	//Crocodile
	const int			__SPAWNPOINT_CROCODILE_MAXSPAWN = 2;
	const float			__SPAWNPOINT_CROCODILE_SPAWN_FREQUENCY = 10.0f;
	const float			__SPAWNPOINT_CROCODILE_WAIT = 2.0f;
	//CakeMonster
	const int			__SPAWNPOINT_CAKEMONSTER_MAXSPAWN = 3;
	const float			__SPAWNPOINT_CAKEMONSTER_SPAWN_FREQUENCY = 5.0f;
	const float			__SPAWNPOINT_CAKEMONSTER_WAIT = 1.5f;
	//Lollipop
	const int			__SPAWNPOINT_LOLLIPOP_MAXSPAWN = 7;
	const float			__SPAWNPOINT_LOLLIPOP_SPAWN_FREQUENCY = 1.0f;
	const float			__SPAWNPOINT_LOLLIPOP_WAIT = 0.5f;

public:
	explicit SpawnPoint(SharedContext* p_sharedContext);
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