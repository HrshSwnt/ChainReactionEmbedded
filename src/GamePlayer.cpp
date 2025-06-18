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

string colorToEscapeCode(const std::string& color, bool cursor) {
    string code;
    if (cursor) {
        code = "\033[47;"; // white background
    } else {
        code = "\033[40;"; // black background
    }
    if (color == "Red") {
        return code + "31m"; // Red
    } else if (color == "Blue") {
        return code + "34m"; // Blue
    } else if (color == "Green") {
        return code + "32m"; // Green
    } else if (color == "Yellow") {
        return code + "33m"; // Yellow
    } else if (color == "Cyan") {
        return code + "36m"; // Cyan
    } else if (color == "Magenta") {
        return code + "35m"; // Magenta
    } else if (color == "Black" && !cursor) {
        return code + "30m"; // Black
    } else if (color == "Black" && cursor) {
        return code + "37m"; // White
    } else {
        return "\033[0m"; // Default color
    }
}