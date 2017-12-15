#pragma once
#include "Actor.h"

enum class QuestionType
{
	TRAVEL = 0,
	ENEMIES_SPAWNED,
	PROJECTILE_SPAWNED,
	LEVEL_DURATION,
	SPAWNPOINTS_SPAWNED
};

class Npc : public Actor
{
private:
	const std::string __NPC_TEXTURE_1 = "Npc_Front_Opened_Mouth";
	const std::string __NPC_TEXTURE_2 = "Npc_Front_Closed_Mouth";
	const float __TALK_DISTANCE = 150;

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

	float m_talkingTimer;
	uint8_t m_currentFrame;
	float m_talkingFrameDuration;

	sf::CircleShape m_talkDistanceDebug;

	float m_speachTimer;
	float m_speachDuration;

	bool m_answer;
	QuestionType m_question;

	float m_questionMarkRotationInc;

	sf::Sprite m_questionMark;
	sf::Sprite m_speachBubble;

	bool m_talking;
};