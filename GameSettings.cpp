#include "main.hpp"

GameSettings::GameSettings() : rows(5), cols(5), players(2) {}

GameSettings::~GameSettings() = default; // Default destructor

GameSettings& GameSettings::instance() {
    static GameSettings instance;  // Thread-safe in C++11+
    return instance;
}

void GameSettings::initializeFromInput() {
    std::string input;
    std::cout << "Enter game settings in the format rows(>4) cols(>4) players(>1): ";
    std::getline(std::cin, input);
    std::vector<std::string> inputSettings;
    std::string temp;

    for (char c : input) {
        if (c == ' ') {
            if (!temp.empty()) {
                inputSettings.push_back(temp);
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        inputSettings.push_back(temp);
    }

    if (inputSettings.size() != 3) {
        std::cerr << "Invalid input. Defaulting to rows=5, cols=5, players=2.\n";
        setDefaults();
        return;
    }

    try {
        int r = std::stoi(inputSettings[0]);
        int c = std::stoi(inputSettings[1]);
        int p = std::stoi(inputSettings[2]);
        if (r > 4 && c > 4 && p > 1) {
            rows = r;
            cols = c;
            players = p;
        } else {
            std::cerr << "Input values out of allowed range. Using defaults.\n";
            setDefaults();
        }
    } catch (...) {
        std::cerr << "Invalid input values. Using defaults.\n";
        setDefaults();
    }
}

void GameSettings::setDefaults() {
    rows = 5;
    cols = 5;
    players = 2;
}
