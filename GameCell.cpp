#include "main.hpp"

GameCell::GameCell() : player(NULL), level(0), row(0), col(0) {}

void GameCell::init(int r, int c) {
    row = r; // Initialize cell with row and column
    col = c;
    player = NULL; // Set player to NULL initially
}
void GameCell::addNeighbor(GameCell* neighbor) {
    neighbors.push_back(neighbor); // Add a neighbor to the cell
}

bool GameCell::reset() {
    bool explosion = false;
    if (((row == 0 || row == GameSettings::instance().rows - 1) &&
        (col == 0 || col == GameSettings::instance().cols - 1)) && level >= 2 && player != NULL) {
        level = 0;
        player = NULL; // Reset cell if it is a corner and level is 2
        explosion = true;
    }
    else if (((row == 0 || row == GameSettings::instance().rows - 1) ||
        (col == 0 || col == GameSettings::instance().cols - 1)) && level >= 3 && player != NULL) {
        level = 0;
        player = NULL; // Reset cell if it is a edge and level is 3
        explosion = true;
    }
    else if (level >= 4 && player != NULL) {
        level = 0;
        player = NULL; // Reset cell if level is 4 or more
        explosion = true;
    }
    return explosion; // Return true if the cell was reset
}

void GameCell::explode(GamePlayer* p){
    player = p; // Set player number if cell is empty
    level++;
    if (reset()){
        // If the cell was reset, notify neighbors
        for (GameCell* neighbor : neighbors) {
            ExplosionQueue::instance().addExplosion(neighbor, p); // Add explosion to the queue
        }
    }
}

bool GameCell::select(GamePlayer* p) {
    if (player != NULL && player->id != p->id) {
        return false; // Return false if the cell is already occupied by another player
    }
    if (player == NULL) {
        player = p; // Set player number if cell is empty
        level = 1; // Set initial level
    } else if (player == p) {
        level++; // Increase level if the same player selects again
        if (reset()) {
            // If the cell was reset, notify neighbors
            for (GameCell* neighbor : neighbors) {
                ExplosionQueue::instance().addExplosion(neighbor, p); // Add explosion to the queue
            }
        }
    }
    return true; // Return true if the cell was successfully selected
    
}