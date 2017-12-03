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

	m_sprite.setScale(1.2f, 1.2f);

	bool spawnIsCorrect;

	uint16_t tries = 0;
	Vector2D<float> newPos;
	ActorManager* actorManager = m_sharedContext->m_actorManager;

	do
	{
		spawnIsCorrect = true;
		newPos.Set(Utils::randomgen(250, 1600), Utils::randomgen(300, 750));

		for (auto it : actorManager->GetSpawnPoints())
			if (newPos.DistanceTo(it->GetPosition()) <= __DISTANCE_MIN_TO_OTHER_SPAWNPOINT && this != it)
				spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetPlayer()->GetPosition()) <= __DISTANCE_MIN_TO_PLAYER)
			spawnIsCorrect = false;

		++tries;
	}
	while (!spawnIsCorrect && tries < 9999);

	m_position = newPos;
	// m_type = SpawnerType::CROCODILE_SPAWNER;
	m_type = static_cast<SpawnerType>(Utils::randomgen(0, 4));
	switch (m_type)
	{
	case SpawnerType::JELLY_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_JELLY_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_JELLY_MAXSPAWN;
		m_secondsBeforeActivation = __SPAWNPOINT_JELLY_WAIT;
		break;
	case SpawnerType::JELLYBEAR_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_JELLYBEAR_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_JELLYBEAR_MAXSPAWN;
		m_secondsBeforeActivation = __SPAWNPOINT_JELLYBEAR_WAIT;
		break;
	case SpawnerType::LOLLIPOP_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_LOLLIPOP_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_LOLLIPOP_MAXSPAWN;
		m_secondsBeforeActivation = __SPAWNPOINT_LOLLIPOP_WAIT;
		break;
	case SpawnerType::CROCODILE_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_CROCODILE_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_CROCODILE_MAXSPAWN;
		m_secondsBeforeActivation = __SPAWNPOINT_CROCODILE_WAIT;
		break;
	case SpawnerType::CAKEMONSTER_SPAWNER:
		m_spawnFrequency = __SPAWNPOINT_CAKEMONSTER_SPAWN_FREQUENCY;
		m_maxSpawn = __SPAWNPOINT_CAKEMONSTER_MAXSPAWN;
		m_secondsBeforeActivation = __SPAWNPOINT_CAKEMONSTER_WAIT;
		break;
	}
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::SpawnEnemy()
{
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
	m_timer += l_time.asSeconds();
	
	if (!IsActive())
		m_activationTimer += l_time.asSeconds();

	if (m_activationTimer >= m_secondsBeforeActivation)
		Activate();

	Actor::Update(l_time);

	if (IsActive() && !IsDone() && m_timer >= m_spawnFrequency)
	{
		SpawnEnemy();
		m_timer = 0;
	}
}

void SpawnPoint::ToggleActive()
{
	m_active = !m_active;
}

void SpawnPoint::Desactivate()
{
	m_active = false;
}

void SpawnPoint::Activate()
{
	m_active = true;
}

void SpawnPoint::Draw() const
{
	if (!IsDone())
		Actor::Draw();
}

bool SpawnPoint::IsDone() const { return m_maxSpawn == 0; }
bool SpawnPoint::IsActive() const { return m_active; }