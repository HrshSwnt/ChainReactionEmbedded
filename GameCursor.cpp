#include "main.hpp"

GameCursor& GameCursor::instance() {
    static GameCursor cursor;
    return cursor;
}

std::mutex& GameCursor::mtx() {
    static std::mutex GameCursorMutex;
    return GameCursorMutex;
}

void GameCursor::init() {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    x = 0;
    y = 0;
}
void GameCursor::moveUp() {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    if (y > 0) --y;
    else y = GameSettings::instance().rows - 1; // Wrap around
}
void GameCursor::moveDown() {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    if (y < GameSettings::instance().rows - 1) ++y;
    else y = 0; // Wrap around
}
void GameCursor::moveLeft() {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    if (x > 0) --x;
    else x = GameSettings::instance().cols - 1; // Wrap around
}
void GameCursor::moveRight() {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    if (x < GameSettings::instance().cols - 1) ++x;
    else x = 0; // Wrap around
}
void GameCursor::setPosition(int newX, int newY) {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    if (newX >= 0 && newX < GameSettings::instance().cols &&
        newY >= 0 && newY < GameSettings::instance().rows) {
        x = newX;
        y = newY;
    }
}
int GameCursor::getX() const {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    return x;
}
int GameCursor::getY() const {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    return y;
}
void GameCursor::reset() {
    std::lock_guard<std::mutex> lock(GameCursor::mtx());
    x = 0;
    y = 0;
}