#include "main.hpp"

GameBoard::GameBoard() : rows(0), cols(0), initialized(false) {}
GameBoard::~GameBoard() = default;

GameBoard& GameBoard::instance() {
    static GameBoard board;
    return board;
}

mutex& GameBoard::mtx() {
    static std::mutex GameBoardMutex;
    return GameBoardMutex;
}


void GameBoard::initialize(int r, int c, int p) {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    if (!initialized) {
        rows = r;
        cols = c;
        players.clear();
        for (int i = 0; i < p; ++i) {
            players.emplace_back(i + 1); // Create players with IDs starting from 1
        }
        currentPlayer = 1; // Start with the first player
        board.resize(rows, vector<GameCell>(cols, GameCell()));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                board[i][j].init(i, j); // Initialize each cell with its row and column
                // Initialize neighbors for each cell
                if (i > 0) board[i][j].addNeighbor(&board[i - 1][j]); // Up
                if (i < rows - 1) board[i][j].addNeighbor(&board[i + 1][j]); // Down
                if (j > 0) board[i][j].addNeighbor(&board[i][j - 1]); // Left
                if (j < cols - 1) board[i][j].addNeighbor(&board[i][j + 1]); // Right
            }
        }
        initialized = true;
    }
}

GamePlayer* GameBoard::getCurrentPlayer(){
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    if (!initialized) {
        std::cerr << "GameBoard not initialized.\n";
        return nullptr; // Return nullptr if not initialized
    }
    return &players[currentPlayer - 1]; // Return the current player
}

void GameBoard::switchPlayer() {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    if (!initialized) {
        std::cerr << "GameBoard not initialized.\n";
        return;
    }
    currentPlayer = (currentPlayer % players.size()) + 1; // Switch to the next player
}


void GameBoard::drawGrid(int x, int y) {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());

    if (!initialized) {
        std::cerr << "GameBoard not initialized.\n";
        return;
    }

    cout << "╔";
    for (int j = 0; j < cols - 1; ++j) {
        cout << "═══╦";
    }
    cout << "═══╗" << endl;

    for (int i = 0; i < rows; ++i) {
        cout << "║";
        for (int j = 0; j < cols; ++j) {
            if (i == y && j == x) {
                cout << "\033[93m";
            }
            cout << " ";
            if (board[i][j].player == 0)
                cout << "█";
            else {
                cout << colorToEscapeCode(board[i][j].player->color); // Use player's color
                cout << board[i][j].level;
                cout << "\033[0m"; // Reset color
            }
            if (i == y && j == x) {
                cout << "\033[0m"; // Reset color
            }
            cout << " ║";
            
        }
        cout << endl;

        if (i != rows - 1) {
            cout << "╠";
            for (int j = 0; j < cols - 1; ++j) {
                cout << "═══╬";
            }
            cout << "═══╣" << endl;
        }
    }

    cout << "╚";
    for (int j = 0; j < cols - 1; ++j) {
        cout << "═══╩";
    }
    cout << "═══╝" << endl;
    cout << "Player " << currentPlayer << "'s turn." << endl;
}
