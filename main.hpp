
using namespace std;

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <termios.h>
#include <unistd.h>

#pragma once

// GameSettings singleton
class GameSettings {
public:
    int rows;
    int cols;
    int players;

    static GameSettings& instance();
    void initializeFromInput();

private:
    GameSettings();
    ~GameSettings();
    void setDefaults();

    GameSettings(const GameSettings&) = delete;
    GameSettings& operator=(const GameSettings&) = delete;
    GameSettings(GameSettings&&) = delete;
    GameSettings& operator=(GameSettings&&) = delete;
};

// GameCell definition
class GameCell {
public:
    int player;
    int level;
    int row;
    int col;
    vector<GameCell*> neighbors;

    GameCell();
    void init(int r, int c);
    void addNeighbor(GameCell* neighbor);
    bool reset();
    void explode(int p);
    bool select(int p);
};

// GameBoard singleton
class GameBoard {
public:
    static GameBoard& instance();
    static mutex& mtx();

    void initialize(int r, int c);
    void drawGrid(int x, int y, int player);

    vector<vector<GameCell>> board;

private:
    GameBoard();
    ~GameBoard();

    GameBoard(const GameBoard&) = delete;
    GameBoard& operator=(const GameBoard&) = delete;
    GameBoard(GameBoard&&) = delete;
    GameBoard& operator=(GameBoard&&) = delete;

    int rows;
    int cols;
    bool initialized;
};

// GameCursor singleton
class GameCursor {
public:
    static GameCursor& instance();
    static mutex& mtx();

    void init();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void setPosition(int newX, int newY);
    int getX() const;
    int getY() const;
    void reset();

private:
    int x;
    int y;
};

// Terminal raw mode utility
void setRawMode(bool enable);