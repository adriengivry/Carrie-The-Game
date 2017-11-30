#include "ActorManager.h"

ActorManager::ActorManager(SharedContext* p_sharedContext) :
	m_sharedContext(p_sharedContext),
	m_player(nullptr) {}

ActorManager::~ActorManager()
{
	for (auto it : m_actors)
		delete it;

	delete m_player;
}

void ActorManager::CreateActors()
{
	const sf::Vector2u windowCenter = m_sharedContext->m_wind->GetWindowCenter();

	m_player = new Player(m_sharedContext, windowCenter.x, windowCenter.y);

	m_actors.push_back(new Enemy(m_sharedContext, 100, 200));
	m_actors.push_back(new Enemy(m_sharedContext, 1400, 800));
	m_actors.push_back(new Enemy(m_sharedContext, 1200, 750));
	m_actors.push_back(new Enemy(m_sharedContext, 800, 100));

	for (auto it : m_actors)
		static_cast<Enemy*>(it)->SetTarget(m_player);
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