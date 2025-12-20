#include "caroLogic.hpp"

caroLogic::caroLogic() {

}

void caroLogic::getBoardSize(int boardRow, int boardCol) {
	m_BoardRow = boardRow;
	m_BoardCol = boardCol;
}

void caroLogic::resetBoard() {
	m_Board.clear();
	m_TurnsTaken = 1;
	m_Turn = playerState::PLAYER_ONE;
}

void caroLogic::initBoard() {
	resetBoard();

	m_Board.resize(m_BoardRow, std::vector<int>(m_BoardCol));

	for (int i = 0; i < m_BoardRow; ++i) {
		for (int j = 0; j < m_BoardCol; ++j) {
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
		m_TurnsTaken++;
		return 1;
	}
}

playerState caroLogic::getTurn() {
	return m_Turn;
}

int caroLogic::getTurnsTaken() {
	return m_TurnsTaken;
}

int caroLogic::countInDirection(int row, int col, int drow, int dcol, int player) {
	int count = 0;
	
	while (row >= 0 && row < m_BoardRow && col >= 0 && col < m_BoardCol && m_Board[row][col] == player) {
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

	for (int i = 0; i < m_BoardRow; ++i) {
		for (int j = 0; j < m_BoardCol; ++j) {
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
	for (int i = 0; i < m_BoardRow; ++i) {
		for (int j = 0; j < m_BoardCol; ++j) {
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

int caroLogic::saveState(const std::filesystem::path& filename) {
	std::ofstream saveFile(filename);

	if (!saveFile.is_open()) {
		return -1;
	}

	saveFile << m_BoardRow << " ";
	saveFile << m_BoardCol << " ";
	saveFile << m_TurnsTaken << " " << std::endl;
	saveFile << std::endl;
	for (auto x : m_Board) {
		for (auto y : x) {
			saveFile << y << " ";
		}
		saveFile << std::endl;
	}

	saveFile.close();
	return 0;
}

int caroLogic::loadState(const std::filesystem::path& filename) {
	std::ifstream saveFile(filename);
	
	if (!saveFile.is_open()) {
		return -1;
	}

	saveFile.seekg(0, std::ifstream::beg);

	if (saveFile >> m_BoardRow >> m_BoardCol) initBoard();
	else return -2;
	saveFile >> m_TurnsTaken;
	if (m_TurnsTaken % 2 == 0) m_Turn = playerState::PLAYER_TWO;
	for (int i = 0; i < m_BoardRow; ++i) {
		for (int j = 0; j < m_BoardCol; ++j) {
			saveFile >> m_Board[i][j];
		}
	}
	
	return 0;
}