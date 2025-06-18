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
    // Initialize game settings
    GameSettings::instance().initializeFromInput();
    system("clear"); // Clear the console screen
    // print the game settings
    cout << "Game Settings:" << endl;
    cout << "Rows: " << GameSettings::instance().rows << endl;
    cout << "Columns: " << GameSettings::instance().cols << endl;
    cout << "Players: " << GameSettings::instance().players << endl;

    setRawMode(true); // Set terminal to raw mode for immediate input handling
    // Initialize the game board and game cursor
    GameCursor::instance().init();
    GameBoard::instance().initialize(GameSettings::instance().rows, GameSettings::instance().cols, GameSettings::instance().players);

    cout << "Game board initialized." << endl;
    cout << "Game players initialized." << endl;
    for (GamePlayer& player : GameBoard::instance().players) {
        cout << "Player " << player.id << " (" << player.color << ") initialized." << endl;
    }
    cout << "Press any key to start the game..." << endl;
    cin.get(); // Wait for user input to start the game
    system("clear"); // Clear the console screen

    DisplayThread::instance().start(); // Start the display thread

    while (true)
    {
        char input;
        read(STDIN_FILENO, &input, 1); // Read a single character input without waiting for Enter key
        switch (input) {
            case 'c':
                system("clear"); // Clear the console screen
                break;
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
            case '\n': // Select the cell at the cursor position
                {
                    int x = GameCursor::instance().getX();
                    int y = GameCursor::instance().getY();
                    if (GameBoard::instance().board[y][x].select(GameBoard::instance().getCurrentPlayer())) {
                        GameBoard::instance().switchPlayer();
                    }
                }
                break;
            case 'q': // Quit the game
                cout << "Exiting game." << endl;
                DisplayThread::instance().stop(); // Stop the display thread
                DisplayThread::instance().join(); // Wait for the display thread to finish
                system("clear"); // Clear the console screen
                cout << "Game ended." << endl;
                cout << "Thank you for playing!" << endl;
                setRawMode(false); // Restore terminal settings before exiting
                return 0;
            default: ;
                // ignore invalid input
                // cout << "Invalid input. Use WASD to move, Q to quit." << endl;
        }
    }
    
    setRawMode(false); // Restore terminal settings before exiting
    cout << "Game ended." << endl;
    
    return 0;
}




