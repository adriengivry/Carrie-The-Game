#include "ActorManager.h"

ActorManager::ActorManager() :
	m_player(nullptr),
	m_npc(nullptr) {}

ActorManager::~ActorManager()
{
}

void ActorManager::AddEnemy(Enemy* p_newActor)
{
	m_enemies.push_back(p_newActor);
}

void ActorManager::AddProjectile(Projectile* p_newActor)
{
	m_projectiles.push_back(p_newActor);
}

void ActorManager::AddSpawnPoint(SpawnPoint* p_newSpawnPoint)
{
	m_spawnPoints.push_back(p_newSpawnPoint);
}

void ActorManager::AddActor(std::multimap<uint16_t&, Actor*>& p_actors, Actor* p_actor)
{
	p_actors.insert(std::pair<uint16_t&, Actor*>(p_actor->GetZBuffer(), p_actor));
}

void ActorManager::SetPlayer(Player* p_player)
{
	m_player = p_player;
}

void ActorManager::SetNpc(Npc* p_npc)
{
	m_npc = p_npc;
}

void ActorManager::SetDoor(const uint8_t p_id, Door* p_door)
{
	m_doors[p_id] = p_door;
}

void ActorManager::CleanActors()
{
	for (auto it : m_enemies)
		delete it;

	for (auto it : m_projectiles)
		delete it;

	for (auto it : m_spawnPoints)
		delete it;

	delete m_doors[0];
	delete m_doors[1];

	delete m_player;
	delete m_npc;

	m_spawnPoints.clear();
	m_enemies.clear();
	m_projectiles.clear();
}

void ActorManager::Update(const sf::Time& l_time)
{
	for (auto it : m_projectiles)
		it->Update(l_time);

	for (auto it : m_spawnPoints)
		it->Update(l_time);

	for (auto it : m_enemies)
		it->Update(l_time);
	

	m_doors[0]->Update(l_time);
	m_doors[1]->Update(l_time);
	m_player->Update(l_time);
	m_npc->Update(l_time);

	CheckDeads();

}

void ActorManager::CheckDeads()
{
	for (auto it = m_enemies.begin(); it != m_enemies.end();)
	{
		if ((*it)->MustDie())
		{
			delete *it;
			it = m_enemies.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = m_projectiles.begin(); it != m_projectiles.end();)
	{
		if ((*it)->MustDie())
		{
			delete *it;
			it = m_projectiles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ActorManager::Draw()
{
	std::multimap<uint16_t&, Actor*> actors;
	AddActor(actors, m_npc);
	AddActor(actors, m_player);
	for (auto it : m_enemies)		AddActor(actors, it);
	for (auto it : m_projectiles)	AddActor(actors, it);

	// ALWAYS DRAW UNDER EVERY OTHER ACTORS
	for (auto it : m_spawnPoints)	it->Draw();
	m_doors[0]->Draw();
	m_doors[1]->Draw();

	// DRAW RESPECTING THEIR ZBUFFER
	for (const auto it : actors)	it.second->Draw();
}

Player* ActorManager::GetPlayer() const { return m_player; }
Npc* ActorManager::GetNpc() const { return m_npc; }

Door* ActorManager::GetDoor(const uint8_t p_id) { return m_doors[p_id]; }
std::vector<Projectile*>& ActorManager::GetProjectile() { return m_projectiles; }
std::vector<Enemy*>& ActorManager::GetEnemies() { return m_enemies; }
std::vector<SpawnPoint*>& ActorManager::GetSpawnPoints() { return m_spawnPoints; }