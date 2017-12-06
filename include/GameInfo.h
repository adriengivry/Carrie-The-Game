#pragma once

enum Curses
{
	REVERSE_MOVEMENT = 0,
	SLOWER_CARRIE,
	SLOWER_PROJECTILES,
	WEAKER_PROJECTILES,
};

enum class MapType
{
	MAP1,
	MAP2
};

struct GameInfo
{
	const float __CARRIE_SLOW_MULTIPLICATOR = 0.8f;
	const float __PROJECTILE_SLOW_MULTIPLICATOR = 0.8f;
	const float __PROJECTILE_WEAK_MULTIPLICATOR = 0.8f;

	uint16_t m_gameStarted = 0;
	const bool m_allowMouse = true;
	const bool m_debugMode = true;

	bool m_gameOver = false;
	uint16_t m_currentLevel = 1;
	bool m_doorPassed = false;
	bool m_levelCompleted = false;
	uint16_t m_spawnedEnemies = 0;
	float m_travelledDistance = 0;
	uint16_t m_spawnedProjectiles = 0;
	bool m_getCursed = false;
	uint8_t m_curseType;

	MapType m_mapType;

	bool m_reverseMovement;
	uint8_t m_slowerCarrie = 0;
	uint8_t m_slowerProjectiles = 0;
	uint8_t m_weakerProjectiles = 0;

	void Reset()
	{
		m_spawnedProjectiles = 0;
		m_spawnedEnemies = 0;
		m_travelledDistance = 0;
		m_gameOver = false;
		m_doorPassed = false;
		m_levelCompleted = false;
		m_getCursed = false;
	}

	void FullReset()
	{
		m_gameStarted = 0;
		m_gameOver = false;
		m_currentLevel = 1;
		m_doorPassed = false;
		m_levelCompleted = false;
		m_spawnedEnemies = 0;
		m_travelledDistance = 0;
		m_spawnedProjectiles = 0;
		m_reverseMovement = false;
		m_slowerCarrie = 0;
		m_slowerProjectiles = 0;
		m_weakerProjectiles = 0;
		m_getCursed = false;
	}
};