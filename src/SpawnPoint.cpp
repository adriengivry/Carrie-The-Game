#include "SpawnPoint.h"
#include "StateManager.h"

SpawnPoint::SpawnPoint(SharedContext * p_sharedContext, const float p_secondBeforeActivation) :
	Actor(p_sharedContext)
{
	SetTexture(__SPAWNPOINT_TEXTURE);
	m_velocity = 0.0f;

	m_maxSpawn = __SPAWNPOINT_MAXSPAWN;
	m_timer = 0;
	m_spawnFrequency = __SPAWNPOINT_SPAWN_FREQUENCY;

	m_secondsBeforeActivation = p_secondBeforeActivation;

	m_active = false;

	bool spawnIsCorrect;

	uint16_t tries = 0;
	Vector2D<float> newPos;
	ActorManager* actorManager = m_sharedContext->m_actorManager;

	do
	{
		spawnIsCorrect = true;
		newPos.Set(Utils::randomgen(400, 1500), Utils::randomgen(250, 750));

		for (auto it : actorManager->GetSpawnPoints())
			if (newPos.DistanceTo(it->GetPosition()) <= __DISTANCE_MIN_TO_OTHER_SPAWNPOINT && this != it)
				spawnIsCorrect = false;

		if (newPos.DistanceTo(actorManager->GetPlayer()->GetPosition()) <= __DISTANCE_MIN_TO_PLAYER)
			spawnIsCorrect = false;

		++tries;
	}
	while (!spawnIsCorrect && tries < 9999);

	m_position = newPos;
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::SpawnEnemy()
{
	m_sharedContext->m_actorManager->AddEnemy(new Enemy(Actor::m_sharedContext, m_position.X(), m_position.Y()));
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
	Actor::Draw();
}

bool SpawnPoint::IsDone() const { return m_maxSpawn == 0; }
bool SpawnPoint::IsActive() const { return m_active; }