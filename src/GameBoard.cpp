#include "main.hpp"

GameBoard::GameBoard() : rows(0), cols(0), initialized(false), firstRun(true), gameOver(false){}
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
        activePlayers.clear();
        for (int i = 0; i < p; ++i) {
            activePlayers.push_back(&players[i]); // Add players to the active players list
        }
        inactivePlayers.clear(); // Clear inactive players list
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
        firstRun = true;
        gameOver = false; // Reset game over state
    }
}

GamePlayer* GameBoard::getCurrentPlayer(){
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    if (!initialized) {
        std::cerr << "GameBoard not initialized.\n";
        return nullptr; // Return nullptr if not initialized
    }
    if (currentPlayer < 1 || currentPlayer > static_cast<int>(activePlayers.size())) {
        std::cerr << "Invalid current player index.\n";
        return nullptr; // Return nullptr if currentPlayer is out of bounds
    }
    return activePlayers[currentPlayer - 1]; // Return the current player
}

void GameBoard::switchPlayer() {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    if (!initialized) {
        std::cerr << "GameBoard not initialized.\n";
        return;
    }
    if (firstRun && currentPlayer == GameSettings::instance().players) {
        firstRun = false; // Set firstRun to false after the first switch
    }
    currentPlayer = (currentPlayer % activePlayers.size()) + 1; // Switch to the next player

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
            cout << " ";
            cout << colorToEscapeCode(board[i][j].player != nullptr ? board[i][j].player->color : "Black", (i == y && j == x));
            if (board[i][j].player == nullptr)
                cout << "█";
            else {
                cout << board[i][j].level;
            }
            cout << "\033[0m"; // Reset color
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
    cout << endl;
    if (inactivePlayers.size() > 0) {
        cout << "Eliminated Players: ";
        for (const auto& player : inactivePlayers) {
            cout << "Player " << player->id << " (" << player->color << ") ";
        }
        cout << endl;
    }
    if (gameOver) {
        cout << "Game Over! Player " << activePlayers[0]->id << " wins!" << endl;
    }
}

vector<vector<string>> GameBoard::getColors() const {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    vector<vector<string>> colors(rows, vector<string>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            colors[i][j] = board[i][j].player ? board[i][j].player->color : "Black";
        }
    }
    return colors;
}

vector<vector<int>> GameBoard::getLevels() const {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    vector<vector<int>> levels(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            levels[i][j] = board[i][j].level;
        }
    }
    return levels;
}

void GameBoard::GameEndCheck()  {
    std::lock_guard<std::mutex> lock(GameBoard::mtx());
    if (!initialized || firstRun) {
        std::cerr << "GameBoard not initialized.\n";
        return ; // Return false if not initialized
    }
    vector<GamePlayer*> newActivePlayers;
    vector<GamePlayer*> newInactivePlayers;
    for (int i = 0; i < GameSettings::instance().players; ++i) {
        bool hasActiveCell = false;
        for (auto& row : board) {
            for (auto& cell : row) {
                if (cell.player == &players[i] && cell.level > 0) {
                    hasActiveCell = true;
                    break;
                }
            }
            if (hasActiveCell) break;
        }
        if (hasActiveCell) {
            newActivePlayers.push_back(&players[i]); // Player has active cells, keep them active
        } else {
            newInactivePlayers.push_back(&players[i]); // Player has no active cells, move to inactive
        }
    }
    activePlayers = newActivePlayers; // Update active players list
    inactivePlayers = newInactivePlayers; // Update inactive players list
    if(activePlayers.size() == 1) gameOver = true; // Game ends if only one player has active cells
    
    
}