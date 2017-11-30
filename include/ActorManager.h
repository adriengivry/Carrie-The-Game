#pragma once

#include <vector>

#include "Player.h"
#include "Enemy.h"

class ActorManager
{
public:
	explicit ActorManager(SharedContext* p_sharedContext);
	~ActorManager();

	void CreateActors();
	void Update(const sf::Time& l_time);
	void Draw();

	Player* GetPlayer() const;
	std::vector<Actor*>& GetActors();

private:
	SharedContext* m_sharedContext;

	Player* m_player;
	std::vector<Actor*> m_actors;
};