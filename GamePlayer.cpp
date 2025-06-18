#include "main.hpp"

GamePlayer::GamePlayer(int id) : id(id), score(0), isActive(true) {
    switch (id) {
        case 1:
            color = "Red";
            break;
        case 2:
            color = "Blue";
            break;
        case 3:
            color = "Green";
            break;
        case 4:
            color = "Yellow";
            break;
        case 5:
            color = "Cyan";
            break;
        case 6:
            color = "Magenta";
            break;
        default:
            color = "Unknown";
    }

}

string colorToEscapeCode(const std::string& color) {
    if (color == "Red") {
        return "\033[31m"; // Red
    } else if (color == "Blue") {
        return "\033[34m"; // Blue
    } else if (color == "Green") {
        return "\033[32m"; // Green
    } else if (color == "Yellow") {
        return "\033[33m"; // Yellow
    } else if (color == "Cyan") {
        return "\033[36m"; // Cyan
    } else if (color == "Magenta") {
        return "\033[35m"; // Magenta
    } else {
        return "\033[0m"; // Default color
    }
}