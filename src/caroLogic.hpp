#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

enum class playerState {
	PLAYER_ONE,
	PLAYER_TWO
};

enum class caroState {
	PLAYER_ONE_WINS,
	PLAYER_TWO_WINS,
	DRAW,
	ONGOING
};

class caroLogic {
public:
	caroLogic();

	void initBoard();
	void resetBoard();
	bool setCell(int, int);
	int getCell(int, int);
	playerState getTurn();

	caroState gameState();

	// playerState m_Turn = playerState::PLAYER_ONE;
private:
	bool isWinner(int, int);
	bool isDraw();
	int countInDirection(int, int, int, int, int);
private:
	std::vector<std::vector<int>> m_Board;

	playerState m_Turn = playerState::PLAYER_ONE;
};