/**
 * @file whack_a_mole.cpp
 * @brief Whack-a-Mole game implementation.
 */

#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <fstream>
#include <pigpio.h>
#include <ncurses.h>
#include <unordered_map>
#include <stdexcept>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

/**
 * @brief Function to select the game mode.
 *
 * @return The selected game mode (1 for Easy, 2 for Medium, 3 for Hard), or -1 for invalid input.
 */
int selectGameMode() {
    int gameMode;
    cout << "Welcome to Whack-a-Mole!\n";
    cout << "Press 1 to Start Game or 0 to Exit: ";
    int startGame;
    if (!(cin >> startGame)) {
        cin.clear(); // clear the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
        cout << "Invalid input. Exiting Game.\n";
        return -1; // Indicates invalid input
    }
    if (startGame != 1) {
        cout << "Exiting Game.\n";
        return -1; // Indicates not starting the game
    }
    cout << "Select Game Mode: \n1. Easy \n2. Medium \n3. Hard\n";
    if (!(cin >> gameMode) || (gameMode < 1 || gameMode > 3)) {
        cin.clear(); // clear the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
        cout << "Invalid game mode. Exiting Game.\n";
        return -1; // Indicates invalid game mode
    }
    return gameMode;
}

// Timer Class

/**
 * @brief Timer class for tracking time during the game.
 */
class Timer {
private:
    int timeLeft;
    chrono::time_point<chrono::system_clock> endTime;

public:
    Timer() : timeLeft(30) {}

    /**
     * @brief Start the timer.
     */
    void start() {
        endTime = chrono::system_clock::now() + chrono::seconds(timeLeft);
        cout << "Timer started!\n";
    }

    /**
     * @brief Stop the timer.
     */
    void stop() {
        cout << "Timer stopped!\n";
    }

    /**
     * @brief Get the remaining time on the timer.
     *
     * @return The remaining time in seconds.
     */
    int getTimeLeft() const {
        auto now = chrono::system_clock::now();
        auto durationLeft = chrono::duration_cast<chrono::seconds>(endTime - now);
        return max(0, static_cast<int>(durationLeft.count()));
    }

    /**
     * @brief Check if the timer has expired.
     *
     * @return True if the timer has expired, false otherwise.
     */
    bool isTimeUp() const {
        return chrono::system_clock::now() >= endTime;
    }
};

// LEDMatrix Class

/**
 * @brief LEDMatrix class for controlling the LED matrix.
 */
class LEDMatrix {
private:
    unordered_map<char, int> keyToLedMap;

public:
    LEDMatrix() {
        // Initialize LED pin numbers
        keyToLedMap = {
                {'4', 15}, {'5', 24}, {'6', 8}, {'7', 20},
                {'r', 14}, {'t', 23}, {'y', 7}, {'u', 21},
                {'f', 3}, {'g', 17}, {'h', 5}, {'j', 19},
                {'v', 2}, {'b', 27}, {'n', 6}, {'m', 26}
        };
    }

    /**
     * @brief Light up a random LED and return its pin number.
     *
     * @param currentLedPin The pin number of the currently lit LED.
     * @return The pin number of the newly lit LED.
     */
    int lightUpRandomLED(int currentLedPin) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, keyToLedMap.size() - 1);

        auto it = keyToLedMap.begin();
        advance(it, distrib(gen));
        int newLedPin = it->second;

        if (currentLedPin != -1) {
            gpioWrite(currentLedPin, 0); // Turn off the current LED
        }

        gpioWrite(newLedPin, 1); // Turn on the new LED
        return newLedPin;
    }

    /**
     * @brief Get the mapping of keys to LED pins.
     *
     * @return The mapping of keys to LED pins.
     */
    const unordered_map<char, int>& getKeyToLedMap() const {
        return keyToLedMap;
    }
};

// Player Class

/**
 * @brief Player class for representing a player in the game.
 */
class Player {
private:
    string name;
    int score;

public:
    Player() : score(0) {}

    /**
     * @brief Set the name of the player.
     *
     * @param n The player's name.
     */
    void setName(const string& n) {
        name = n;
    }

    /**
     * @brief Get the name of the player.
     *
     * @return The player's name.
     */
    string getName() const {
        return name;
    }

    /**
     * @brief Get the player's score.
     *
     * @return The player's score.
     */
    int getScore() const {
        return score;
    }

    /**
     * @brief Increment the player's score.
     */
    void incrementScore() {
        score++;
    }

    /**
     * @brief Decrement the player's score (if greater than 0).
     */
    void decrementScore() {
        if (score > 0) score--;
    }
};

// HighScore Class

/**
 * @brief HighScore class for managing high scores in the game.
 */
class HighScore {
private:
    map<int, vector<pair<string, int>>> leaderboards;

public:
    /**
     * @brief Add a high score entry.
     *
     * @param score The player's score.
     * @param playerName The player's name.
     */
    void add(int score, string playerName) {
        ofstream outputFile("highScores.txt", ios::app);
        if (outputFile.is_open()) {
            outputFile << endl << playerName << " " << score;
            outputFile.close();
        } else {
            cerr << "Unable to open the file for writing." << endl;
        }
    }

    /**
     * @brief Print the high scores to the console.
     */
    void print() {
        ifstream inputFile("highScores.txt");
        string name;
        int score;
        multimap<int, string> highScoresMap;

        if (inputFile.is_open()) {
            while (inputFile >> name >> score) {
                highScoresMap.insert(make_pair(score, name));
            }
            inputFile.close();
        } else {
            cerr << "Unable to open the file." << endl;
        }

        cout << "Name " << "Score" << endl;
        int count = 0;
        for (auto rit = highScoresMap.rbegin(); rit != highScoresMap.rend() && count < 5; ++rit) {
            cout << rit->second << " " << rit->first << endl;
            count++;
        }
    }
};

// GameController Class

/**
 * @brief GameController class for managing the game flow.
 */
class GameController {
private:
    Timer timer;
    LEDMatrix ledMatrix;
    Player currentPlayer;

    int lightUpRandomLEDAndWaitForInput(int currentLedPin);
    int lightUpRandomLEDNoWait(int currentLedPin);
    vector<int> lightUpMultipleRandomLEDs(vector<int>& activeLeds);

public:
    /**
     * @brief Start the game.
     */
    void startGame() {
        cout << "Game started!\n";
        timer.start();
    }

    /**
     * @brief Perform initial setup for the game.
     */
    void setup() {
        if (gpioInitialise() < 0) {
            throw runtime_error("Failed to initialize pigpio.");
        }

        const auto& keyToLedMap = ledMatrix.getKeyToLedMap();
        for (auto const& kvp : keyToLedMap) {
            gpioSetMode(kvp.second, PI_OUTPUT);
            gpioWrite(kvp.second, 0); // Initially turn off all LEDs
        }
    }

    /**
     * @brief Run the main game loop.
     *
     * @param player The player object.
     * @param highScore The highScore object.
     * @param gameMode The selected game mode.
     */
    void inGame(Player& player, HighScore& highScore, int gameMode) {
        setup(); // Setup using LEDMatrix
        initscr();
        noecho();
        cbreak();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        curs_set(0);

        int currentLedPin = -1;
        vector<int> activeLeds;

        timer.start();
        while (!timer.isTimeUp()) {
            mvprintw(0, 0, "Time left: %d seconds ", timer.getTimeLeft());
            mvprintw(1, 0, "Score: %d", player.getScore());
            refresh();

            switch (gameMode) {
                case 1: // Easy
                    currentLedPin = lightUpRandomLEDAndWaitForInput(currentLedPin);
                    break;
                case 2: // Medium
                    currentLedPin = lightUpRandomLEDNoWait(currentLedPin);
                    break;
                case 3: // Hard
                    activeLeds = lightUpMultipleRandomLEDs(activeLeds);
                    break;
                default:
                    // Default behavior or error handling
                    break;
            }
        }

        endwin();
        gpioTerminate();
        cout << "Game Over! " << player.getName() << ", your final score is: " << player.getScore() << endl;
        highScore.add(player.getScore(), player.getName());
    }

    /**
     * @brief End the game and display the final score.
     *
     * @param player The player object.
     */
    void endGame(Player& player) {
        cout << "Game ended!\n";
        timer.stop();
        cout << "Final score: " << player.getScore() << "\n";
    }
};

/**
 * @brief Main function for running the Whack-a-Mole game.
 *
 * @return 0 on successful execution.
 */
int main() {
    HighScore highScore;
    GameController game;
    Player player;
    int gameMode;
    int choice;

    bool running = true;
    while (running) {
        cout << "Whack-a-LED Game\n";
        cout << "1. Start Game\n";
        cout << "2. Instructions\n";
        cout << "3. High Scores\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                gameMode = selectGameMode();
                game.startGame();
                game.inGame(player, highScore, gameMode);
                game.endGame(player);
                break;
            case 2:
                cout << "Instructions:\n";
                cout << "- The LED matrix will light up in random patterns.\n";
                cout << "- Your goal is to 'hit' the lit LED by pressing the enter key as soon as it lights up.\n";
                cout << "- The faster you hit, the more points you score.\n";
                cout << "- The keybinds for the 4x4 matrix are set as follows:\n";
                cout << "- 4  5  6  7\n";
                cout << "- r  t  y  u\n";
                cout << "- f  g  h  j\n";
                cout << "- v  b  n  m\n";
                cout << "- You have 30 seconds to score as many points as possible.\n\n";
                break;
            case 3:
                cout << "High Scores: \n";
                highScore.print();
                getch();
                break;
            case 4:
                running = false;
                break;
            default:
                cout << "Invalid choice, please try again.\n";
                break;
        }

        // Clear the input buffer in case of invalid input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining input
        }
    }

    return 0;
}
