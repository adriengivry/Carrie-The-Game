#pragma once

enum Curses
{
	REVERSE_MOVEMENT = 0,
	SLOWER_CARRIE,
	SLOWER_PROJECTILES,
	WEAKER_PROJECTILES,
};

struct GameInfo
{
	const float __CARRIE_SLOW_MULTIPLICATOR = 0.9;
	const float __PROJECTILE_SLOW_MULTIPLICATOR = 0.9;
	const float __PROJECTILE_WEAK_MULTIPLICATOR = 0.9;

	uint16_t m_gameStarted = 0;
	const bool m_allowMouse = true;
	bool m_gameOver = false;
	uint16_t m_currentLevel = 1;
	bool m_doorPassed = false;
	bool m_levelCompleted = false;
	uint16_t m_spawnedEnemies = 0;
	float m_travelledDistance = 0;
	uint16_t m_spawnedProjectiles = 0;

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
	}
};