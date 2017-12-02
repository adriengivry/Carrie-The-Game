#pragma once

struct GameInfo
{
	uint16_t m_gameStarted = 0;
	const bool m_allowMouse = true;
	bool m_gameOver = false;

	uint8_t m_reverseMovement = 0;
	uint8_t m_lowerAttackSpeed = 0;
	uint8_t m_midLife = 0;
	uint8_t m_slowerProjectiles = 0;
	uint8_t m_weakerProjectiles = 0;
	uint8_t m_strongerEnemies = 0;
};