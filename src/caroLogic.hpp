#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

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

	void getBoardSize(int, int);
	void initBoard();
	void resetBoard();
	bool setCell(int, int);
	int getCell(int, int);
	playerState getTurn();
	int getTurnsTaken();

	caroState gameState();

	int saveState(const std::filesystem::path &);
	int loadState(const std::filesystem::path&);

	// playerState m_Turn = playerState::PLAYER_ONE;
private:
	bool isWinner(int, int);
	bool isDraw();
	int countInDirection(int, int, int, int, int);
private:
	std::vector<std::vector<int>> m_Board;

	int m_BoardRow = 0;
	int m_BoardCol = 0;

	playerState m_Turn = playerState::PLAYER_ONE;

	int m_TurnsTaken = 1;
};