#include "caroLogic.hpp"

caroLogic::caroLogic() {

}

void caroLogic::resetBoard() {
	m_Board.clear();

	m_Turn = playerState::PLAYER_ONE;
}

void caroLogic::initBoard() {
	resetBoard();

	m_Board.resize(8, std::vector<int>(8));

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			m_Board[i][j] = 0;
		}
	}
}

int caroLogic::getCell(int r, int c) {
	return m_Board[r][c];
}

bool caroLogic::setCell(int r, int c) {
	if (getCell(r, c) != 0) return 0;
	else {
		if (m_Turn == playerState::PLAYER_ONE) {
			m_Board[r][c] = 1;
			m_Turn = playerState::PLAYER_TWO;
		}
		else if (m_Turn == playerState::PLAYER_TWO) {
			m_Board[r][c] = 2;
			m_Turn = playerState::PLAYER_ONE;
		}
		return 1;
	}
}

playerState caroLogic::getTurn() {
	return m_Turn;
}

int caroLogic::countInDirection(int row, int col, int drow, int dcol, int player) {
	int count = 0;
	
	while (row >= 0 && row < 8 && col >= 0 && col < 8 && m_Board[row][col] == player) {
		++count;
		row += drow;
		col += dcol;
	}

	return count;
}

bool caroLogic::isWinner(int player, int k) {
	// player: 1 is x, 2 is o

	int direction[4][2] = {
		{0, 1},
		{1, 0},
		{1, 1},
		{1, -1}
	};

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (m_Board[i][j] == 0) continue;

			for (auto& d : direction) {
				int dRow = d[0];
				int dCol = d[1];

				int characterCount = countInDirection(i, j, dRow, dCol, player);

				if (characterCount >= k) return true;
			}
		}
	}
	return false;
}

bool caroLogic::isDraw() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (m_Board[i][j] == 0) return false;
		}
	}
	return true;
}

caroState caroLogic::gameState() {
	if (isWinner(1, 4)) return caroState::PLAYER_ONE_WINS;
	if (isWinner(2, 4)) return caroState::PLAYER_TWO_WINS;
	if (isDraw()) return caroState::DRAW;
	return caroState::ONGOING;
}