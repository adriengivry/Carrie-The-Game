#include "ActorManager.h"

ActorManager::ActorManager() :
	m_player(nullptr) {}

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

void ActorManager::SetPlayer(Player* p_player)
{
	m_player = p_player;
}

void ActorManager::SetNpc(Npc* p_npc)
{
	m_npc = p_npc;
}

void ActorManager::CleanActors()
{
	for (auto it : m_enemies)
		delete it;

	for (auto it : m_projectiles)
		delete it;

	delete m_player;
	delete m_npc;

	m_enemies.clear();
	m_projectiles.clear();
}

void ActorManager::Update(const sf::Time& l_time)
{
	CheckDeads();

	for (auto it : m_enemies)
		it->Update(l_time);

	for (auto it : m_projectiles)
		it->Update(l_time);

	m_player->Update(l_time);
	m_npc->Update(l_time);
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
	for (auto it : m_enemies)
		it->Draw();
	
	for (auto it : m_projectiles)
		it->Draw();

	m_npc->Draw();
	m_player->Draw();
}

Player* ActorManager::GetPlayer() const { return m_player; }
Npc* ActorManager::GetNpc() const { return m_npc; }

std::vector<Projectile*>& ActorManager::GetProjectile() { return m_projectiles; }
std::vector<Enemy*>& ActorManager::GetEnemies() { return m_enemies; }
