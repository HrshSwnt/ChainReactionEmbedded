#include "main.hpp"


DisplayThread::DisplayThread() : running(false) {}
DisplayThread::~DisplayThread() {
    stop();
}

DisplayThread& DisplayThread::instance() {
    static DisplayThread instance; // Singleton instance
    return instance;
}

void DisplayThread::start() {
    if (running && displayThread.joinable()) {
        return; // If already running, do nothing
    }
    if (!running) {
        running = true;
        displayThread = std::thread(&DisplayThread::run, this);
    }
}

void DisplayThread::stop() {
    if (running) {
        running = false;
        if (displayThread.joinable()) {
            displayThread.join();
        }
    }
}

bool DisplayThread::isRunning() const {
    return running;
}

void DisplayThread::join() {
    if (!running) {
        return; // If not running, do nothing
    }
    running = false; // Set running to false to stop the thread
    if (displayThread.joinable()) {
        displayThread.join();
    }
}

void DisplayThread::run() {
    running = true; // Set running to true to indicate the thread is active
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust the sleep duration as needed
        system("clear"); // Clear the console screen
        if (!GameFrameQueue::instance().isEmpty()) {
            GameFrame frame = GameFrameQueue::instance().getNextFrame(); // Get the next frame from the queue
            drawGameFrame(frame); // Draw the game frame
        } else {
            drawGameFrame({
                GameCursor::instance().getX(),
                GameCursor::instance().getY(),
                GameBoard::instance().getCurrentPlayer()->id,
                GameBoard::instance().getColors(),
                GameBoard::instance().getLevels(),
                GameBoard::instance().inactivePlayers, // List of eliminated players
                GameBoard::instance().gameOver // Flag to indicate if the game is over
            });
        }
    }
}