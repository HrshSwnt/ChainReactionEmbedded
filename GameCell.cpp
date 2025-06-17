#include "main.hpp"

GameCell::GameCell() : player(0), level(0), row(0), col(0) {}

void GameCell::init(int r, int c) {
    row = r; // Initialize cell with row and column
    col = c;
}
void GameCell::addNeighbor(GameCell* neighbor) {
    neighbors.push_back(neighbor); // Add a neighbor to the cell
}

bool GameCell::reset() {
    bool explosion = false;
    if (((row == 0 || row == GameSettings::instance().rows - 1) &&
        (col == 0 || col == GameSettings::instance().cols - 1)) && level >= 2) {
        level = 0;
        player = 0; // Reset cell if it is a corner and level is 2
        explosion = true;
    }
    else if (((row == 0 || row == GameSettings::instance().rows - 1) ||
        (col == 0 || col == GameSettings::instance().cols - 1)) && level >= 3) {
        level = 0;
        player = 0; // Reset cell if it is a edge and level is 3
        explosion = true;
    }
    else if (level >= 4) {
        level = 0;
        player = 0; // Reset cell if level is 4 or more
        explosion = true;
    }
    return explosion; // Return true if the cell was reset
}

void GameCell::explode(int p){
    player = p; // Set player number if cell is empty
    level++;
    if (reset()){
        // If the cell was reset, notify neighbors
        for (GameCell* neighbor : neighbors) {
            neighbor->explode(p); // Notify neighbors to explode
        }
    }
}

bool GameCell::select(int p) {
    if (player != 0 && player != p) {
        return false; // Return false if the cell is already occupied by another player
    }
    if (player == 0) {
        player = p; // Set player number if cell is empty
        level = 1; // Set initial level
    } else if (player == p) {
        level++; // Increase level if the same player selects again
        if (reset()) {
            // If the cell was reset, notify neighbors
            for (GameCell* neighbor : neighbors) {
                neighbor->explode(p); // Notify neighbors to explode
            }
        }
    }
    return true; // Return true if the cell was successfully selected
    
}