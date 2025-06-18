#include "main.hpp"

ExplosionProcessorThread::ExplosionProcessorThread() : running(false) {}
ExplosionProcessorThread::~ExplosionProcessorThread() {
    stop();
}
ExplosionProcessorThread& ExplosionProcessorThread::instance() {
    static ExplosionProcessorThread instance; // Singleton instance
    return instance;
}
void ExplosionProcessorThread::start() {
    if (running && explosionThread.joinable()) {
        return; // If already running, do nothing
    }
    if (!running) {
        running = true;
        explosionThread = std::thread(&ExplosionProcessorThread::run, this);
    }
}
void ExplosionProcessorThread::stop() {
    if (running) {
        running = false;
        if (explosionThread.joinable()) {
            explosionThread.join();
        }
    }
}
bool ExplosionProcessorThread::isRunning() const {
    return running;
}
void ExplosionProcessorThread::join() {
    if (!running) {
        return; // If not running, do nothing
    }
    running = false; // Set running to false to stop the thread
    if (explosionThread.joinable()) {
        explosionThread.join();
    }
}
void ExplosionProcessorThread::run() {
    running = true; // Set running to true to indicate the thread is active
    while (running) {
        processExplosions(); // Process explosions in the game board
    }
}
void ExplosionProcessorThread::processExplosions() {
    std::unique_lock<std::mutex> lock(ExplosionQueue::mtx()); // Lock the mutex for thread safety
    ExplosionQueue::cv().wait_for(lock, chrono::milliseconds(100), [] {
        return !ExplosionQueue::instance().explosionQueue.empty(); // Wait until there are explosions to process
    });
    if (!running) {
        return; // If the thread is not running, exit
    }
    if (!ExplosionQueue::instance().explosionQueue.empty()) {
        Explosion exp = ExplosionQueue::instance().explosionQueue.front(); // Get the first explosion from the queue
        ExplosionQueue::instance().explosionQueue.pop(); // Remove the explosion from the queue
        lock.unlock(); // Unlock the mutex before processing the explosion
        exp.cell->explode(exp.player); // Process the explosion in the cell
    }
    
}