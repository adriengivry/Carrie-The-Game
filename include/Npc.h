#pragma once
#include "Actor.h"

class Npc : public Actor
{
private:
	const std::string __NPC_TEXTURE = "Npc";
	const float __TALK_DISTANCE = 130;

public:
	explicit Npc(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Npc();

	void Activate();
	void Desactivate();

	bool IsTalking() const;
	bool IsActive() const;

	void Update(const sf::Time& l_time) override;
	void Draw() const override;

	void DrawAffirmation() const;

private:
	bool m_isActive;

	bool m_talking;
};