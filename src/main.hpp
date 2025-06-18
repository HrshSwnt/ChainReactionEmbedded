
using namespace std;

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
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
    vector<vector<string>> getColors() const;
    vector<vector<int>> getLevels() const;
    void GameEndCheck();

    vector<vector<GameCell>> board;
    vector<GamePlayer> players;
    vector<GamePlayer*> activePlayers;
    vector<GamePlayer*> inactivePlayers;
    int rows;
    int cols;
    bool initialized;
    bool firstRun;
    bool gameOver;

private:
    GameBoard();
    ~GameBoard();

    GameBoard(const GameBoard&) = delete;
    GameBoard& operator=(const GameBoard&) = delete;
    GameBoard(GameBoard&&) = delete;
    GameBoard& operator=(GameBoard&&) = delete;

    int currentPlayer;
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

class DisplayThread{
public:
    static DisplayThread& instance();
    void start();
    void stop();
    void join();
    bool isRunning() const;

private:
    DisplayThread();
    ~DisplayThread();
    DisplayThread(const DisplayThread&) = delete;
    DisplayThread& operator=(const DisplayThread&) = delete;
    DisplayThread(DisplayThread&&) = delete;
    DisplayThread& operator=(DisplayThread&&) = delete;
    thread displayThread;
    bool running;
    void run();
};

struct Explosion {
    GameCell* cell;
    GamePlayer* player;
};

class ExplosionQueue {
public:
    static ExplosionQueue& instance();
    static mutex& mtx();
    static condition_variable& cv();
    void addExplosion(GameCell* cell, GamePlayer* player);
    void processExplosions();
    void clear();
    queue<Explosion> explosionQueue;
private:
    ExplosionQueue();
    ~ExplosionQueue();
    ExplosionQueue(const ExplosionQueue&) = delete;
    ExplosionQueue& operator=(const ExplosionQueue&) = delete;
    ExplosionQueue(ExplosionQueue&&) = delete;
    ExplosionQueue& operator=(ExplosionQueue&&) = delete;
};



class ExplosionProcessorThread {
public:
    static ExplosionProcessorThread& instance();
    void start();
    void stop();
    void join();
    void processExplosions();
    bool isRunning() const;
private:
    ExplosionProcessorThread();
    ~ExplosionProcessorThread();
    ExplosionProcessorThread(const ExplosionProcessorThread&) = delete;
    ExplosionProcessorThread& operator=(const ExplosionProcessorThread&) = delete;
    ExplosionProcessorThread(ExplosionProcessorThread&&) = delete;
    ExplosionProcessorThread& operator=(ExplosionProcessorThread&&) = delete;
    
    thread explosionThread;
    bool running;
    void run();
};

struct GameFrame {
    int cursorX;
    int cursorY;
    int currentPlayerId;
    vector<vector<string>> colors;
    vector<vector<int>> levels;
    vector<GamePlayer*> eliminatedPlayers; // List of eliminated players
    bool gameOver; // Flag to indicate if the game is over
};

class GameFrameQueue{
public:
    static GameFrameQueue& instance();
    static mutex& mtx();
    static condition_variable& cv();
    void addFrame(const GameFrame& frame);
    GameFrame getNextFrame();
    bool isEmpty() const;
    void clear();

private:
    GameFrameQueue();
    ~GameFrameQueue();
    GameFrameQueue(const GameFrameQueue&) = delete;
    GameFrameQueue& operator=(const GameFrameQueue&) = delete;
    GameFrameQueue(GameFrameQueue&&) = delete;
    GameFrameQueue& operator=(GameFrameQueue&&) = delete;

    queue<GameFrame> frameQueue;
};


// Terminal raw mode utility
void setRawMode(bool enable);
string colorToEscapeCode(const string& color, bool cursor);
void drawGameFrame(const GameFrame& frame);
GameFrame createGameFrame(int cursorX, int cursorY, int currentPlayerId);