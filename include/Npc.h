#pragma once
#include "Actor.h"

enum class QuestionType
{
	TRAVEL = 0,
	ENEMIES_SPAWNED,
	PROJECTILE_SPAWNED
};

class Npc : public Actor
{
private:
	const std::string __NPC_TEXTURE = "Npc";
	const float __TALK_DISTANCE = 5000;

public:
	explicit Npc(SharedContext* p_sharedContext, const float p_x = 0, const float p_y = 0);
	~Npc();

	void Activate();
	void Desactivate();

	void GenerateQuestion();

	bool IsTalking() const;
	bool IsActive() const;

	bool GetAnswer() const;

	void Update(const sf::Time& l_time) override;
	void Draw() const override;

	void DrawAffirmation() const;

private:
	bool m_isActive;

	uint16_t m_randomValue;

	bool m_answer;
	QuestionType m_question;

	sf::Sprite m_speachBubble;

	bool m_talking;
};