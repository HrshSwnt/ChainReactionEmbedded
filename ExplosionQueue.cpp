#include "main.hpp"

ExplosionQueue::ExplosionQueue() {}

ExplosionQueue::~ExplosionQueue() {
    clear(); // Clear the queue when the destructor is called
}

ExplosionQueue& ExplosionQueue::instance() {
    static ExplosionQueue instance; // Singleton instance
    return instance;
}
std::mutex& ExplosionQueue::mtx() {
    static std::mutex explosionMutex; // Mutex for thread safety
    return explosionMutex;
}
std::condition_variable& ExplosionQueue::cv() {
    static std::condition_variable explosionCV; // Condition variable for synchronization
    return explosionCV;
}

void ExplosionQueue::addExplosion(GameCell* cell, GamePlayer* player) {
    std::lock_guard<std::mutex> lock(mtx()); // Lock the mutex for thread safety
    instance().explosionQueue.push({cell, player}); // Add the explosion to the queue
    cv().notify_one(); // Notify one waiting thread that an explosion has been added
}

void ExplosionQueue::clear() {
    std::lock_guard<std::mutex> lock(mtx()); // Lock the mutex for thread safety
    while (!explosionQueue.empty()) {
        explosionQueue.pop(); // Clear the queue by popping all elements
    }
    cv().notify_all(); // Notify all waiting threads that the queue has been cleared
}
