#pragma once

enum Curses
{
	REVERSE_MOVEMENT = 0,
	SLOWER_CARRIE,
	SLOWER_PROJECTILES,
	WEAKER_PROJECTILES,
	REDUCED_PRECISION
};

enum class MapType
{
	MAP1,
	MAP2,
	MAP_BOSS
};

enum class GameMode
{
	ARCADE,
	BOSS_RUSH
};

struct GameInfo
{
	const float __CARRIE_SLOW_MULTIPLICATOR = 0.8f;
	const float __PROJECTILE_SLOW_MULTIPLICATOR = 0.8f;
	const float __PROJECTILE_WEAK_MULTIPLICATOR = 0.8f;
	const float __PLAYER_LIFE = 100;

	GameMode m_gameMode = GameMode::ARCADE;

	float m_playerMaxLife = __PLAYER_LIFE;
	float m_playerLife = __PLAYER_LIFE;

	uint16_t m_gameStarted = 0;
	const bool m_allowMouse = true;
	const bool m_debugMode = false;

	uint16_t m_toothPaste = 0;

	bool m_gameOver = false;
	uint16_t m_currentLevel = 1;
	bool m_doorPassed = false;
	bool m_levelCompleted = false;
	bool m_questionAsked = false;
	uint16_t m_spawnedEnemies = 0;
	float m_travelledDistance = 0;
	uint16_t m_spawnedProjectiles = 0;
	float m_levelDuration = 0;
	uint16_t m_spawnedSpawnPoints = 0;
	bool m_getCursed = false;
	uint8_t m_curseType;

	uint8_t m_extraLifeBought = 0;
	uint8_t m_curingPotionBought = 0;
	uint8_t m_smallCuringPotionBought = 0;

	bool m_bossBeaten;

	MapType m_mapType;

	bool m_reverseMovement;
	uint8_t m_slowerCarrie = 0;
	uint8_t m_slowerProjectiles = 0;
	uint8_t m_weakerProjectiles = 0;
	uint8_t m_reducedPrecision = 0;

	void Reset()
	{
		m_levelDuration = 0;
		m_spawnedProjectiles = 0;
		m_spawnedEnemies = 0;
		m_travelledDistance = 0;
		m_spawnedSpawnPoints = 0;
		m_gameOver = false;
		m_doorPassed = false;
		m_levelCompleted = false;
		m_getCursed = false;
		m_questionAsked = false;
	}

	void FullReset()
	{
		m_toothPaste = 0;
		m_levelDuration = 0;
		m_gameStarted = 0;
		m_gameOver = false;
		m_currentLevel = 1;
		m_doorPassed = false;
		m_levelCompleted = false;
		m_spawnedEnemies = 0;
		m_travelledDistance = 0;
		m_spawnedProjectiles = 0;
		m_spawnedSpawnPoints = 0;
		m_reverseMovement = false;
		m_slowerCarrie = 0;
		m_slowerProjectiles = 0;
		m_weakerProjectiles = 0;
		m_reducedPrecision = 0;
		m_getCursed = false;
		m_questionAsked = false;
		m_playerMaxLife = __PLAYER_LIFE;
		m_playerLife = __PLAYER_LIFE;
		m_bossBeaten = false;
	}
};