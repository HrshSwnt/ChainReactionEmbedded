
using namespace std;

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <termios.h>
#include <unistd.h>

#pragma once

class GamePlayer {
public:
    int id;
    int score;
    string color;
    bool isActive;

    GamePlayer(int id);

};


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
    GamePlayer* player;
    int level;
    int row;
    int col;
    vector<GameCell*> neighbors;

    GameCell();
    void init(int r, int c);
    void addNeighbor(GameCell* neighbor);
    bool reset();
    void explode(GamePlayer* p);
    bool select(GamePlayer* p);
};

// GameBoard singleton
class GameBoard {
public:
    static GameBoard& instance();
    static mutex& mtx();

    void initialize(int r, int c, int p);
    void drawGrid(int x, int y);
    void switchPlayer();
    GamePlayer* getCurrentPlayer();

    vector<vector<GameCell>> board;
    vector<GamePlayer> players;

private:
    GameBoard();
    ~GameBoard();

    GameBoard(const GameBoard&) = delete;
    GameBoard& operator=(const GameBoard&) = delete;
    GameBoard(GameBoard&&) = delete;
    GameBoard& operator=(GameBoard&&) = delete;

    int rows;
    int cols;
    int currentPlayer;
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
string colorToEscapeCode(const string& color);