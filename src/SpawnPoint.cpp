#include "SpawnPoint.h"
#include "StateManager.h"
#include "Jelly.h"
#include "JellyBear.h"
#include "CakeMonster.h"
#include "Crocodile.h"
#include "Lollipop.h"


SpawnPoint::SpawnPoint(SharedContext * p_sharedContext) :
	Actor(p_sharedContext)
{
	SetTexture(__SPAWNPOINT_TEXTURE);
	m_velocity = 0.0f;

	m_timer = 0;

	m_gotAShadow = false;

	m_active = false;

	m_activationTimer = 0;

	m_secondsBeforeActivation = Utils::randomgen(2, 6);

	m_sprite.setScale(1.2f, 1.2f);

	bool spawnIsCorrect;

	uint16_t tries = 0;
	Vector2D<float> newPos;
	ActorManager* actorManager = m_sharedContext->m_actorManager;

	do
	{
		spawnIsCorrect = true;
		newPos.Set(Utils::randomgen(250, 1600), Utils::randomgen(325, 950));

		for (auto it : actorManager->GetSpawnPoints())
			if (newPos.DistanceTo(it->GetPosition()) <= __DISTANCE_MIN_TO_OTHER_SPAWNPOINT && this != it)
				spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetDoor(0)->GetPosition()) <= __DISTANCE_MIN_TO_DOOR)
			spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetDoor(1)->GetPosition()) <= __DISTANCE_MIN_TO_DOOR)
			spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetPlayer()->GetPosition()) <= __DISTANCE_MIN_TO_PLAYER)
			spawnIsCorrect = false;

		++tries;
	}
	while (!spawnIsCorrect && tries < 9999);

	m_futurePosition = newPos;
	m_position = newPos;
	m_position.Y(-100);
	// m_type = SpawnerType::CAKEMONSTER_SPAWNER;
	m_type = static_cast<SpawnerType>(Utils::randomgen(0, 4));
	switch (m_type)
	{
	case SpawnerType::JELLY_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_JELLY_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_JELLY_MAXSPAWN;
		break;
	case SpawnerType::JELLYBEAR_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_JELLYBEAR_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_JELLYBEAR_MAXSPAWN;
		break;
	case SpawnerType::LOLLIPOP_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_LOLLIPOP_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_LOLLIPOP_MAXSPAWN;
		break;
	case SpawnerType::CROCODILE_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_CROCODILE_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_CROCODILE_MAXSPAWN;
		break;
	case SpawnerType::CAKEMONSTER_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_CAKEMONSTER_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_CAKEMONSTER_MAXSPAWN;
		break;
	}
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::SpawnEnemy()
{
	m_timer = 0;

	switch (m_type)
	{
	case SpawnerType::JELLY_SPAWNER :
		m_sharedContext->m_actorManager->AddEnemy(new Jelly(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::JELLYBEAR_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new JellyBear(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::LOLLIPOP_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new Lollipop(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::CROCODILE_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new Crocodile(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	case SpawnerType::CAKEMONSTER_SPAWNER:
		m_sharedContext->m_actorManager->AddEnemy(new CakeMonster(Actor::m_sharedContext, m_position.X(), m_position.Y()));
		break;
	}
	--m_maxSpawn;
}

void SpawnPoint::Update(const sf::Time & l_time)
{
	if (IsActive())
	{
		m_timer += l_time.asSeconds();
		m_position = m_futurePosition;
	}

	
	if (!IsActive())
	{
		m_activationTimer += l_time.asSeconds();
		if (m_activationTimer >= m_secondsBeforeActivation - 1 && m_position.Y() < m_futurePosition.Y())
		{
			m_position.Y() += 1500 * l_time.asSeconds();
		}
		
		if (m_activationTimer >= m_secondsBeforeActivation)
			Activate();
	}

	for (auto it : m_sharedContext->m_actorManager->GetEnemies())
		if (m_position.DistanceTo(it->GetPosition()) <= 50)
			m_timer = 0.f;

	if (IsActive() && !IsDone() && m_timer >= m_spawnFrequency)
		SpawnEnemy();

	Actor::Update(l_time);
}

void SpawnPoint::ToggleActive()
{
	m_active = !m_active;
}

void SpawnPoint::Desactivate()
{
	if (m_active)
		m_active = false;
}

void SpawnPoint::Activate()
{
	if (!m_active)
	{
		m_active = true;
		m_timer = m_spawnFrequency;
	}
}

void SpawnPoint::Draw() const
{
	if (!IsDone())
		Actor::Draw();
}

bool SpawnPoint::IsDone() const { return m_maxSpawn == 0; }
bool SpawnPoint::IsActive() const { return m_active; }