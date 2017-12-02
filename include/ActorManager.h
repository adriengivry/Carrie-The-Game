#pragma once

#include <vector>

#include "Actor.h"
#include "Player.h"
#include "Npc.h"
#include "Enemy.h"
#include "Projectile.h"

class ActorManager
{
public:
	ActorManager();
	~ActorManager();

	void AddEnemy(Enemy* p_newActor);
	void AddProjectile(Projectile* p_newActor);
	void SetPlayer(Player* p_player);
	void SetNpc(Npc* p_npc);
	void CleanActors();
	void Update(const sf::Time& l_time);
	void CheckDeads();
	void Draw();

	Player* GetPlayer() const;
	Npc* GetNpc() const;
	std::vector<Projectile*>& GetProjectile();
	std::vector<Enemy*>& GetEnemies();

private:
	Player* m_player;
	Npc* m_npc;
	std::vector<Enemy*> m_enemies;
	std::vector<Projectile*> m_projectiles;
};