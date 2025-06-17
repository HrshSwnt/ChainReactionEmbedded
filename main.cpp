#include "main.hpp"


void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt); // Save current terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
        std::cout << "\033[?25l"; // Hide cursor
    } else {
        std::cout << "\033[?25h"; // Show cursor
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original settings
    }
}

int main() {
    setRawMode(true); // Set terminal to raw mode for immediate input handling
    // Initialize game settings
    GameSettings::instance().initializeFromInput();
    system("clear"); // Clear the console screen
    // print the game settings
    cout << "Game Settings:" << endl;
    cout << "Rows: " << GameSettings::instance().rows << endl;
    cout << "Columns: " << GameSettings::instance().cols << endl;
    cout << "Players: " << GameSettings::instance().players << endl;

    // Initialize the game board and game cursor
    GameCursor::instance().init();
    GameBoard::instance().initialize(GameSettings::instance().rows, GameSettings::instance().cols);
    
    cout << "Game board initialized." << endl;
    cout << "Press any key to start the game..." << endl;
    cin.get(); // Wait for user input to start the game
    system("clear"); // Clear the console screen

    int currentPlayer = 1; // Start with player 1

    while (true)
    {
        system("clear"); // Clear the console screen
        GameBoard::instance().drawGrid(GameCursor::instance().getX(), GameCursor::instance().getY(), currentPlayer);
        char input;
        read(STDIN_FILENO, &input, 1); // Read a single character input without waiting for Enter key
        switch (input) {
            case 'w': // Move cursor up
                GameCursor::instance().moveUp();
                break;
            case 's': // Move cursor down
                GameCursor::instance().moveDown();
                break;
            case 'a': // Move cursor left
                GameCursor::instance().moveLeft();
                break;
            case 'd': // Move cursor right
                GameCursor::instance().moveRight();
                break;
            case 'q': // Quit the game
                cout << "Exiting game." << endl;
                setRawMode(false); // Restore terminal settings before exiting
                return 0;
            case '\n': // Select the cell at the cursor position
                {
                    std::lock_guard<std::mutex> lock(GameBoard::mtx());
                    int x = GameCursor::instance().getX();
                    int y = GameCursor::instance().getY();
                    if (GameBoard::instance().board[y][x].select(currentPlayer)) {
                        currentPlayer = (currentPlayer % GameSettings::instance().players) + 1; // Switch to the next player
                    }
                }
                break;
            default: ;
                // ignore invalid input
                // cout << "Invalid input. Use WASD to move, Q to quit." << endl;
        }
    }
    
    setRawMode(false); // Restore terminal settings before exiting
    cout << "Game ended." << endl;
    
    return 0;
}




