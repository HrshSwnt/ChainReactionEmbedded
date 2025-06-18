#include "main.hpp"

GameFrameQueue::GameFrameQueue(){}
GameFrameQueue::~GameFrameQueue() {
    clear(); // Clear the queue when the destructor is called
}
GameFrameQueue& GameFrameQueue::instance() {
    static GameFrameQueue instance; // Singleton instance
    return instance;
}
std::mutex& GameFrameQueue::mtx() {
    static std::mutex frameMutex; // Mutex for thread safety
    return frameMutex;
}
std::condition_variable& GameFrameQueue::cv() {
    static std::condition_variable frameCV; // Condition variable for synchronization
    return frameCV;
}
void GameFrameQueue::addFrame(const GameFrame& frame) {
    std::lock_guard<std::mutex> lock(mtx()); // Lock the mutex for thread safety
    instance().frameQueue.push(frame); // Add the frame to the queue
    cv().notify_one(); // Notify one waiting thread that a frame has been added
}
GameFrame GameFrameQueue::getNextFrame() {
    std::unique_lock<std::mutex> lock(mtx()); // Lock the mutex for thread safety
    cv().wait(lock, [] { return !instance().frameQueue.empty(); }); // Wait until there is a frame to process
    GameFrame frame = instance().frameQueue.front(); // Get the front frame from the queue
    instance().frameQueue.pop(); // Remove the frame from the queue
    return frame; // Return the frame
}
bool GameFrameQueue::isEmpty() const {
    std::lock_guard<std::mutex> lock(mtx()); // Lock the mutex for thread safety
    return instance().frameQueue.empty(); // Check if the queue is empty
}
void GameFrameQueue::clear() {
    std::lock_guard<std::mutex> lock(mtx()); // Lock the mutex for thread safety
    while (!instance().frameQueue.empty()) {
        instance().frameQueue.pop(); // Clear the queue by popping all elements
    }
    cv().notify_all(); // Notify all waiting threads that the queue has been cleared
}

void drawGameFrame(const GameFrame& frame) {
   std::lock_guard<std::mutex> lock(GameBoard::mtx());
    cout << "╔";
    int rows = GameSettings::instance().rows;
    int cols = GameSettings::instance().cols;
    int x = frame.cursorX;
    int y = frame.cursorY;
    int currentPlayer = frame.currentPlayerId;
    for (int j = 0; j < cols - 1; ++j) {
        cout << "═══╦";
    }
    cout << "═══╗" << endl;

    for (int i = 0; i < rows; ++i) {
        cout << "║";
        for (int j = 0; j < cols; ++j) {
            cout << " ";
            cout << colorToEscapeCode(frame.colors[i][j], (i == y && j == x));
            if (frame.levels[i][j] == 0)
                cout << "█";
            else {
                cout << frame.levels[i][j];
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
    if (!frame.eliminatedPlayers.empty()) {
        cout << "Eliminated Players: ";
        for (const auto& player : frame.eliminatedPlayers) {
            cout << "Player " << player->id << " (" << player->color << ") " << endl;
        }
        cout << endl;
    }
    if (frame.gameOver) {
        cout << "Game Over! Player " << frame.eliminatedPlayers[0]->id << " wins!" << endl;
    }
}

GameFrame createGameFrame() {
    GameFrame frame;
    frame.cursorX = GameCursor::instance().getX();
    frame.cursorY = GameCursor::instance().getY();
    frame.currentPlayerId = GameBoard::instance().getCurrentPlayer()->id;

    int rows = GameSettings::instance().rows;
    int cols = GameSettings::instance().cols;

    frame.colors.resize(rows, vector<string>(cols));
    frame.levels.resize(rows, vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (GameBoard::instance().board[i][j].player != nullptr) {
                frame.colors[i][j] = GameBoard::instance().board[i][j].player->color;
            } else {
                frame.colors[i][j] = "Black";
            }
            frame.levels[i][j] = GameBoard::instance().board[i][j].level;
        }
    }

    return frame;
}