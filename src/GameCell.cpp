#include "main.hpp"

GameCell::GameCell() : player(nullptr), level(0), row(0), col(0) {}

void GameCell::init(int r, int c) {
    row = r; // Initialize cell with row and column
    col = c;
    player = nullptr; // Set player to nullptr initially
}
void GameCell::addNeighbor(GameCell* neighbor) {
    neighbors.push_back(neighbor); // Add a neighbor to the cell
}

bool GameCell::reset() {
    bool explosion = false;
    if (((row == 0 || row == GameSettings::instance().rows - 1) &&
        (col == 0 || col == GameSettings::instance().cols - 1)) && level >= 2 && player != nullptr) {
        level = 0;
        player = nullptr; // Reset cell if it is a corner and level is 2
        explosion = true;
    }
    else if (((row == 0 || row == GameSettings::instance().rows - 1) ||
        (col == 0 || col == GameSettings::instance().cols - 1)) && level >= 3 && player != nullptr) {
        level = 0;
        player = nullptr; // Reset cell if it is a edge and level is 3
        explosion = true;
    }
    else if (level >= 4 && player != nullptr) {
        level = 0;
        player = nullptr; // Reset cell if level is 4 or more
        explosion = true;
    }
    return explosion; // Return true if the cell was reset
}

void GameCell::explode(GamePlayer* p){
    player = p; // Set player number if cell is empty
    level++;
    GameBoard::instance().GameEndCheck();
    if (reset()){
        GameFrameQueue::instance().addFrame({
            GameCursor::instance().getX(),
            GameCursor::instance().getY(),
            p->id,
            GameBoard::instance().getColors(),
            GameBoard::instance().getLevels(),
            GameBoard::instance().inactivePlayers, // List of eliminated players
            GameBoard::instance().gameOver // Flag to indicate if the game is over
        }); // Add frame to the queue
        // If the cell was reset, notify neighbors
        for (GameCell* neighbor : neighbors) {
            ExplosionQueue::instance().addExplosion(neighbor, p); // Add explosion to the queue
        }
    }
}

bool GameCell::select(GamePlayer* p) {
    if (player != nullptr && player->id != p->id) {
        return false; // Return false if the cell is already occupied by another player
    }
    if (player == nullptr) {
        player = p; // Set player number if cell is empty
        level = 1; // Set initial level
        GameFrameQueue::instance().addFrame({
            GameCursor::instance().getX(),
            GameCursor::instance().getY(),
            p->id,
            GameBoard::instance().getColors(),
            GameBoard::instance().getLevels(),
            GameBoard::instance().inactivePlayers, // List of eliminated players
            GameBoard::instance().gameOver // Flag to indicate if the game is over
        }); // Add frame to the queue
    } else if (player == p) {
        level++; // Increase level if the same player selects again
        if (reset()) {
            GameFrameQueue::instance().addFrame({
                GameCursor::instance().getX(),
                GameCursor::instance().getY(),
                p->id,
                GameBoard::instance().getColors(),
                GameBoard::instance().getLevels(),
                GameBoard::instance().inactivePlayers, // List of eliminated players
                GameBoard::instance().gameOver // Flag to indicate if the game is over
            }); // Add frame to the queue
            // If the cell was reset, notify neighbors
            for (GameCell* neighbor : neighbors) {
                ExplosionQueue::instance().addExplosion(neighbor, p); // Add explosion to the queue
            }
        }
    }
    return true; // Return true if the cell was successfully selected
    
}