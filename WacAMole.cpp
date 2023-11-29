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
 * Function to select the game mode.
 * @return int The selected game mode or -1 on invalid input or exit.
 */
int selectGameMode() {
    int gameMode;
    cout << "Welcome to Whack-a-Mole!\n";
    cout << "Press 1 to Start Game or 0 to Exit: ";
    int startGame;
    if (!(cin >> startGame)) {
        cin.clear(); // clear the error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
        cout << "Invalid game mode. Exiting Game.\n";
        return -1; // Indicates invalid game mode
    }
    return gameMode;
}

/**
 * Timer class for managing countdown in the game.
 */
class Timer {
private:
    int timeLeft;
    chrono::time_point<chrono::system_clock> endTime;

public:
    Timer() : timeLeft(30) {}

    void start() {
        endTime = chrono::system_clock::now() + chrono::seconds(timeLeft);
        cout << "Timer started!\n";
    }

    void stop() {
        cout << "Timer stopped!\n";
    }

    int getTimeLeft() const {
        auto now = chrono::system_clock::now();
        auto durationLeft = chrono::duration_cast<chrono::seconds>(endTime - now);
        return max(0, static_cast<int>(durationLeft.count()));
    }

    bool isTimeUp() const {
        return chrono::system_clock::now() >= endTime;
    }
};

/**
 * LEDMatrix class for managing the LED matrix in the game.
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

    const unordered_map<char, int>& getKeyToLedMap() const {
        return keyToLedMap;
    }
};

/**
 * Player class for managing player details.
 */
class Player {
private:
    string name;
    int score;

public:
    Player() : score(0) {}

    void setName(const string& n) {
        name = n;
    }

    string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    void incrementScore() {
        score++;
    }

    void decrementScore() {
        if (score > 0) score--;
    }
};

/**
 * HighScore class for managing high scores in the game.
 */
class HighScore {
private:
    map<int, vector<pair<string, int>>> leaderboards;

public:
    void add(int score, string playerName) {
        ofstream outputFile("highScores.txt", ios::app);
        if (outputFile.is_open()) {
            outputFile << endl << playerName << " " << score;
