#pragma once

#include <vector>

#include "Actor.h"
#include "Player.h"

class ActorManager
{
public:
	ActorManager();
	~ActorManager();

	void AddActor(Actor* p_newActor);
	void SetPlayer(Player* p_player);
	void CleanActors();
	void Update(const sf::Time& l_time);
	void CheckDeads();
	void Draw();

	Player* GetPlayer() const;
	std::vector<Actor*>& GetActors();

private:
	Player* m_player;
	std::vector<Actor*> m_actors;
};