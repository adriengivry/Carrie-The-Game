#include "ActorManager.h"

ActorManager::ActorManager() :
	m_player(nullptr) {}

ActorManager::~ActorManager()
{
}

void ActorManager::AddActor(Actor* p_newActor)
{
	m_actors.push_back(p_newActor);
}

void ActorManager::SetPlayer(Player* p_player)
{
	m_player = p_player;
}

void ActorManager::CleanActors()
{
	for (auto it : m_actors)
		delete it;

	delete m_player;

	m_actors.clear();
}

void ActorManager::Update(const sf::Time& l_time)
{
	for (auto it : m_actors)
		it->Update(l_time);

	m_player->Update(l_time);
}

void ActorManager::Draw()
{
	for (auto it : m_actors)
		it->Draw();

	m_player->Draw();
}

Player* ActorManager::GetPlayer() const { return m_player; }
std::vector<Actor*>& ActorManager::GetActors() { return m_actors; }